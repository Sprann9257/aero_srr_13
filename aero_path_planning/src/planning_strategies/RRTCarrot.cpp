/**
 * @file RRTCarrot.cpp
 *
 * @date   Feb 19, 2013
 * @author parallels
 * @brief \todo
 */

//License File

//****************SYSTEM DEPENDANCIES**************************//
#include<boost/foreach.hpp>
#include<boost/random.hpp>
#include<boost/random/normal_distribution.hpp>
#include<pcl/common/distances.h>
#include<pcl/common/norms.h>
#include <boost/math/special_functions/round.hpp>
//*****************LOCAL DEPENDANCIES**************************//
#include<aero_path_planning/planning_strategies/RRTCarrot.h>
#include <aero_path_planning/utilities/AeroPathPlanningUtilities.h>
//**********************NAMESPACES*****************************//


using namespace aero_path_planning;

//*******************RRTCarrotTree*****************************//

RRTCarrotTree::RRTCarrotTree(const RRTCarrotTree& copy):
								nodes_(copy.nodes_)
{

}

RRTCarrotTree::RRTCarrotTree(const RRTCarrotTree* copy):
							nodes_(copy->nodes_)
{

}

RRTCarrotTree::RRTCarrotTree():
								nodes_()
{

}

RRTCarrotTree::~RRTCarrotTree()
{
	this->flushTree();
};

bool RRTCarrotTree::addNode(const node_ptr_t node)
{
	this->nodes_.push_back(node);
	return true;
}

const node_ptr_t& RRTCarrotTree::findNearestNeighbor(const node_ptr_t to_node)const
{
	double best_distance = std::numeric_limits<double>::max();
	double dist;
	const node_ptr_t* nearest  = NULL;
	for(node_deque::const_iterator node_itr = this->nodes_.begin(); node_itr<this->nodes_.end(); node_itr++)
	{
		dist = to_node->location_.distance((*node_itr)->location_);
		if(dist<best_distance)
		{
			best_distance =  dist;
			nearest       = &(*node_itr);
		}
	}
	if(nearest!=NULL) return *nearest;
	else              return this->empty_node_;
}

const node_ptr_t& RRTCarrotTree::getLeafNode()
{
	if(this->nodes_.size()>0)
	{
		return this->nodes_.back();
	}
	else
	{
		return this->empty_node_;
	}
}

const node_ptr_t& RRTCarrotTree::getRootNode()
{
	if(this->nodes_.size()>0)
	{
		return this->nodes_.front();
	}
	else
	{
		return this->empty_node_;
	}
}

RRTCarrotTree::size_type RRTCarrotTree::size() const
{
	return this->nodes_.size();
}

void RRTCarrotTree::flushTree()
{
	BOOST_FOREACH(node_deque::value_type node, this->nodes_)
			{
		node.reset();
			}
	this->nodes_.clear();
}

RRTCarrotTree& RRTCarrotTree::operator=(RRTCarrotTree const &copy)
{
	this->nodes_ = copy.nodes_;
	return *this;
}

//*********************RRTCarrot*******************************//

RRTCarrot::RRTCarrot(const aero_path_planning::RRTCarrot& copy):
						step_size_(copy.step_size_),
						initialized_(copy.initialized_),
						has_delta_(copy.has_delta_),
						has_coll_(copy.has_coll_),
						has_map_(copy.has_map_),
						delta_(copy.delta_),
						map_(copy.map_),
						collision_checker_(copy.collision_checker_),
						start_tree_(NULL),
						goal_tree_(NULL),
						rand_gen_(NULL)
{
	if(copy.start_tree_!=NULL)
	{
		this->start_tree_  = new RRTCarrotTree(*copy.start_tree_);
	}
	if(copy.goal_tree_!=NULL)
	{
		this->goal_tree_   = new RRTCarrotTree(*copy.goal_tree_);
	}
	this->randInit();
}

RRTCarrot::RRTCarrot():
						step_size_(0),
						initialized_(false),
						has_delta_(false),
						has_coll_(false),
						has_map_(false),
						delta_(0),
						start_tree_(NULL),
						goal_tree_(NULL),
						rand_gen_(NULL)
{
	this->randInit();
}

RRTCarrot::RRTCarrot(double step_size):
						step_size_(step_size),
						initialized_(false),
						has_delta_(false),
						has_coll_(false),
						has_map_(false),
						delta_(0),
						start_tree_(NULL),
						goal_tree_(NULL),
						rand_gen_(NULL)
{
	this->randInit();
}

RRTCarrot::~RRTCarrot()
{
	if(this->start_tree_!=NULL) delete this->start_tree_;
	if(this->goal_tree_ !=NULL) delete this->goal_tree_;
	if(this->rand_gen_  !=NULL) delete this->rand_gen_;
}

void RRTCarrot::randInit()
{
	boost::mt19937 RNG;
	boost::normal_distribution<> dist(0, 1);
	this->rand_gen_  = new boost::variate_generator<boost::mt19937, boost::normal_distribution<> >(RNG, dist);
	this->rand_gen_->engine().seed();
	this->rand_gen_->distribution().reset();
}

bool RRTCarrot::seedSampler(double seed)
{
	this->rand_gen_->engine().seed(seed);
	this->rand_gen_->distribution().reset();
	return true;
}

bool RRTCarrot::isInialized()
{
	this->initialized_ = this->has_coll_&&this->has_delta_&&this->has_map_;
	return this->initialized_;
}

bool RRTCarrot::setCollision(collision_func_& collision_checker)
{
	this->collision_checker_ = collision_checker;
	this->has_coll_ = true;
	this->isInialized();
	return true;
}

bool RRTCarrot::setSearchMap(occupancy_grid::MultiTraitOccupancyGridConstPtr map)
{
	this->map_ = map;
	this->has_map_ = true;
	//Since assiging a new map would invalidate any trees we've built, delete them if they exist
	if(this->start_tree_!=NULL)
	{
		delete this->start_tree_;
	}
	this->start_tree_ = new RRTCarrotTree();
	if(this->goal_tree_!=NULL)
	{
		delete this->goal_tree_;
	}
	this->goal_tree_  = new RRTCarrotTree();
	this->isInialized();
	return true;
}

bool RRTCarrot::setCarrotDelta(double delta)
{
	this->delta_ = delta;
	this->has_delta_ = true;
	this->isInialized();
	return true;
}

bool RRTCarrot::sample(node_ptr_t& node)
{
	if(this->initialized_)
	{
		int x, y, z;
		this->genLoc(&x, &y, &z);
		node->location_.setX(x);
		node->location_.setY(y);
		node->location_.setZ(z);
		tf::Point origin_point;
		tf::pointMsgToTF(this->map_->getOrigin().position, origin_point);
		this->map_->getOrigin();
		node->location_ -= origin_point;
		return true;
	}
	else return false;
}

void RRTCarrot::genLoc(int* x, int* y, int* z)
{
	*x = this->map_->getXSizeGrid()*(*this->rand_gen_)();
	if(*x<0)
	{
		*x=0;
	}
	else if(*x>=this->map_->getXSizeGrid())
	{
		*x=this->map_->getXSizeGrid()-1;
	}
	*y = this->map_->getYSizeGrid()*(*this->rand_gen_)();
	if(*y<0)
	{
		*y=0;
	}
	else if(*y>=this->map_->getYSizeGrid())
	{
		*y=this->map_->getYSizeGrid()-1;
	}
	*z = 0;
}

bool RRTCarrot::connect(const node_ptr_t& q_rand, const node_ptr_t& tree_node, RRTCarrotTree* tree)
{
	//If the node is on the tree already, we already searched it, don't add it
	if(this->nodesEqual(q_rand, tree_node)) return false;
	//If the node is less than one step size away from q_rand, we've already connected
	if((q_rand->location_-tree_node->location_).length()<this->step_size_)
	{
		return true;
	}

	ROS_INFO_STREAM("Attempting to connect ("<<tree_node->location_.x()<<","<<tree_node->location_.y()<<
			") to ("<<q_rand->location_.x()<<","<<q_rand->location_.y()<<")");

	//Set up the connection properties
	aero_path_planning::PointCloud line_cloud;

	aero_path_planning::Point tree_point;
	aero_path_planning::Point sample_point;
	aero_path_planning::vectorToPoint(tree_node->location_, tree_point);
	aero_path_planning::vectorToPoint(q_rand->location_, sample_point);
	aero_path_planning::castLine(tree_point, sample_point, 0, line_cloud);
	bool connected = true;
	line_cloud.erase(line_cloud.begin());
	//While we're less than one step-size away from q_rand, step along the line
	tf::Point line_point;
	BOOST_FOREACH(aero_path_planning::Point next_point, line_cloud)
	{
		aero_path_planning::pointToVector(next_point, line_point);
		//Make a step and see if it's in collision
		if(!this->collision_checker_(line_point, *this->map_))
		{
			node_ptr_t next_node(new RRTNode());
			next_node->location_= line_point;
			next_node->parent_  = tree->getLeafNode();
			tree->addNode(next_node);
		}
		else
		{
			//If it was in collision, we're done
			connected = false;
			break;
		}

	}
	//If we got through the whole line, we sucessfully connected the nodes. Need to add q_rand because the castLine function doesn't include the endpoints
	if(connected)
	{
		node_ptr_t q_rand_l (new RRTNode());
		q_rand_l->location_= q_rand->location_;
		q_rand_l->parent_  = tree->getLeafNode();
		tree->addNode(q_rand_l);
	}
	return connected;
}

bool RRTCarrot::step(const node_ptr_t& last_node, const tf::Vector3& step_vector, node_ptr_t& next_node)
{
	if(step_vector.length()<this->step_size_) return false;
	tf::Point next_point;
	next_point = last_node->location_+step_vector;


	if(!this->collision_checker_(next_point, *this->map_))
	{
		next_node->location_= next_point;
		next_node->parent_  = last_node;
		return true;
	}
	return false;
}

//bool RRTCarrot::generateStepVector(const Point& from_point, const Point& to_point, Eigen::Vector4f& vector) const
//{
//	vector       =  to_point.getVector4fMap()-from_point.getVector4fMap();
//	double scale =  ((double)this->step_size_)/vector.norm();
//	vector       *= scale;
//	Point round_temp;
//	round_temp.getVector4fMap() = vector;
//	round_temp.x = boost::math::round(round_temp.x);
//	round_temp.y = boost::math::round(round_temp.y);
//	round_temp.z = boost::math::round(round_temp.z);
//	vector       = round_temp.getVector4fMap();
//	return true;
//}

bool RRTCarrot::allowsPartialPath()
{
	return true;
}

bool RRTCarrot::search(const geometry_msgs::Pose& start_point, const geometry_msgs::Pose& goal_point, ros::Duration& timeout, std::deque<geometry_msgs::Pose>& result_path)
{
	if(this->initialized_)
	{
		std::string search_type;
		this->getPlanningType(search_type);
		std::stringstream start_str;
		start_str<<"("<<start_point.position.x<<","<<start_point.position.y<<")";
		std::stringstream goal_str;
		goal_str<<"("<<goal_point.position.x<<","<<goal_point.position.y<<")";
		ROS_INFO_STREAM("I'm Performing a "<<search_type<<" Search from start_point <"<<start_str.str()<<"> to <"<<goal_str.str()<<">");

		//Get the current time for timeout checks
		ros::Time start_time = ros::Time::now();

		//Build the initial nodes, add to trees
		bool success = false;
		bool time_out = false;

		tf::Point start;
		tf::Point goal;
		tf::pointMsgToTF(start_point.position, start);
		tf::pointMsgToTF(goal_point.position, goal);

		node_ptr_t start_node (new RRTNode());
		start_node->parent_ = node_ptr_t();
		start_node->location_= start;
		node_ptr_t goal_node(new RRTNode());
		goal_node->parent_  = node_ptr_t();
		goal_node->location_ = goal;

		this->start_tree_->flushTree();
		this->start_tree_->addNode(start_node);
		this->goal_tree_->flushTree();
		this->goal_tree_->addNode(goal_node);

		bool from_start = true; //Flag to signal if we're sampling randomly from start_tree or goal_tree
		RRTCarrotTree* sample_tree = this->start_tree_;
		RRTCarrotTree* connect_tree= this->goal_tree_;

		ROS_INFO_STREAM("Tree Set Up, Beginning Search!");

		while(!success&&!time_out)
		{
			//Sample a new random point on the grid
			node_ptr_t q_rand(new RRTNode());
			q_rand->parent_ = node_ptr_t();
			this->sample(q_rand);

			//debug
			std::stringstream node_str;
			node_str<<"("<<q_rand->location_.x()<<","<<q_rand->location_.y()<<")";
			ROS_INFO_STREAM("Sampling Point "<<node_str.str());


			//Check to see if the point is in collision
			if(!this->collision_checker_(q_rand->location_, *this->map_))
			{
				//debug
				ROS_INFO_STREAM("Got a valid sample point!");

				//Swap which tree we're sampling from and witch tree we're connecting to as needed
				if(!from_start)
				{
					std::swap(sample_tree, connect_tree);
				}

				//Connect the nearest neighbor on the sampling tree to the sampled point
				this->connect(q_rand, sample_tree->findNearestNeighbor(q_rand), sample_tree);
				//Attempt to connect the sample tree to the connect tree, if sucessfull we're done as we have a path
				success = this->connect(sample_tree->getLeafNode(), connect_tree->findNearestNeighbor(sample_tree->getLeafNode()), connect_tree);
				if(!success)
			    {
					from_start = !from_start;
			    }
			}
			else
			{
				ROS_INFO("Sampled Point Was In Collision");
			}
			time_out = (ros::Time::now()-start_time)>timeout;
			//debug
			ROS_INFO_STREAM("Tree Sizes Thus Far: start_tree="<<this->start_tree_->size()<<" nodes, goal_tree= "<<this->goal_tree_->size()<<" nodes");

		}
		ROS_INFO_STREAM("Connected The Trees or ROS Quit, Building Path If ROS Running...");
		//Merge the paths together. As the last nodes added to each tree will have been the connect nodes, we use those as entires to merge
		if(!from_start)
		{
			//Make sure we're building a path from the start to the goal
			std::swap(sample_tree, connect_tree);
		}

		//Build the final complete path
		node_ptr_t next_node_on_path;
		//If we didn't timeout, merge the trees
		if(!time_out)
		{
			ROS_INFO("Merging Paths...");
			if(!this->mergePath(sample_tree->getLeafNode(), connect_tree->getLeafNode()))
			{
				ROS_ERROR("Something Went Wrong Merging Paths!");
				return false;
			}
			next_node_on_path = connect_tree->getRootNode();
		}
		//Otherwise build the path that got closest to goal
		else
		{
			ROS_INFO("Building Incomplete Path...");
			next_node_on_path = sample_tree->findNearestNeighbor(goal_node);
		}

		geometry_msgs::Pose path_pose;
		path_pose.orientation.w=1;
		while(next_node_on_path != node_ptr_t())
		{
			//ROS_INFO_STREAM("Adding Node At ("<<next_node_on_path->location_.x<<","<<next_node_on_path->location_.y<<")");
			tf::pointTFToMsg(next_node_on_path->location_, path_pose.position);
			result_path.push_back(path_pose);
			next_node_on_path = next_node_on_path->parent_;
		}
		//Built the path or terminated early, we're done
		return success;
	}
	else
	{
		ROS_ERROR("Cannot Perform Path Planning On Uninitialized Planner!");
		return false;
	}
}

bool RRTCarrot::getPlanningType(std::string& type) const
{
	type = "RRT Carrot Planner";
	return true;
}


bool RRTCarrot::mergePath(node_ptr_t path_1_node, node_ptr_t path_2_node)
{
	//ROS_INFO_STREAM("Merging Path With Bridge At ("<<path_1_node->location_.x<<"."<<path_1_node->location_.y<<
	//		") and ("<<path_2_node->location_.x<<","<<path_2_node->location_.y<<")");
	if(path_2_node == node_ptr_t() || path_1_node == node_ptr_t())
	{
		return false;
	}
	node_ptr_t new_parent_node(path_1_node);
	node_ptr_t new_child_node(path_2_node);
	node_ptr_t swap;
	while(new_child_node!=node_ptr_t())
	{
		//ROS_INFO_STREAM("Connecting ("<<new_child_node->location_.x<<"."<<new_child_node->location_.y<<
		//		") and ("<<new_parent_node->location_.x<<","<<new_parent_node->location_.y<<")");
		swap                    = new_child_node->parent_;
		new_child_node->parent_ = new_parent_node;
		new_parent_node         = new_child_node;
		new_child_node          = swap;

	}
	return true;
}

bool RRTCarrot::nodesEqual(const node_ptr_t& node1, const node_ptr_t& node2)
{
	bool equal_x = node1->location_.x() == node2->location_.x();
	bool equal_y = node1->location_.y() == node2->location_.y();
	bool equal_z = node1->location_.z() == node2->location_.z();
	return equal_x&&equal_y&&equal_z;
}


RRTCarrot& RRTCarrot::operator=(RRTCarrot const &copy)
{
	if(this->start_tree_!=NULL&&this->start_tree_!=copy.start_tree_)
	{
		delete this->start_tree_;
		this->start_tree_  = new RRTCarrotTree(copy.start_tree_);
	}
	else if(this->start_tree_==NULL)
	{
		this->start_tree_  = new RRTCarrotTree(copy.start_tree_);
	}
	if(this->goal_tree_!=NULL&&this->goal_tree_!=copy.goal_tree_)
	{
		delete this->goal_tree_;
		this->goal_tree_   = new RRTCarrotTree(copy.goal_tree_);
	}
	else if(this->goal_tree_==NULL)
	{
		this->goal_tree_   = new RRTCarrotTree(copy.goal_tree_);
	}
	this->initialized_ = copy.initialized_;
	this->has_delta_   = copy.has_delta_;
	this->has_coll_    = copy.has_coll_;
	this->has_map_     = copy.has_map_;
	this->delta_       = copy.delta_;
	this->map_         = copy.map_;
	this->collision_checker_ = copy.collision_checker_;

	return *this;
}
