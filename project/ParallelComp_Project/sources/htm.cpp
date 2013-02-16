#include "../includes/htm.hpp"

// ADD POINT
void ICoDF_HTM::HTM::AddPoint(const double& ra, const double& dec)
{
	PointInfo_t* info = new PointInfo_t;
	info->_ra = ra;
	info->_dec = dec;
    info->_position = Eigen::Vector3d(0, 0, 0);
    if (IsCorrectRA(ra) && IsCorrectDEC(dec))
    {
        if (info->_position == Eigen::Vector3d(0, 0, 0))
        {
            double rProjection = sin(90 - abs(dec));
            double x = rProjection * cos(ra);
            double y = rProjection * sin(ra);
            double z = cos(90 - abs(dec));
            info->_position = Eigen::Vector3d(x, y, z);
        }
    }
	if (this->SelectRootTrixel(info))
		this->_pointList.push(info);
	else
		delete info;
}

// CreateHTM
bool ICoDF_HTM::HTM::CreateHTM()
{
	PointInfo_t* pt;
	while (!this->_pointList.empty())
    {
		pt = _pointList.front();
		this->_pointList.pop();
		this->AssignPoint(pt);
    }
	return true;
}

// AssignPoint
void ICoDF_HTM::HTM::AssignPoint(PointInfo_t* pt)
{
	if (pt->_current->_nbChildObject > 1)
    {
		unsigned short int index = GetIndex(pt->_current, pt);
		if (index == (unsigned int short)~0)
			return;
		
		if (pt->_current->_children[index] == NULL)
			pt->_current->_children[index] = CreateTrixelChild(pt->_current, index);
        pt->_current = pt->_current->_children[index];
        this->_pointList.push(pt);
		
    }
	else if (pt->_current->_nbChildObject == 1)
    {
		++pt->_current->_nbChildObject;
		unsigned short int indexCurrent = GetIndex(pt->_current, pt);
		PointInfo_t* old = pt->_current->_info;
		pt->_current->_info = NULL;
		unsigned short int indexOld = GetIndex(old->_current, old);
		if (indexCurrent == (unsigned short int)~0 ||
            indexOld == (unsigned short int)~0)
			return;
		
		if (pt->_current->_children == NULL)
		{
			CreateTrixelChildren(pt->_current);
			CreateTrixelChild(pt->_current, indexOld);
			if (indexOld != indexCurrent)
				CreateTrixelChild(pt->_current, indexCurrent);
			auto it = this->_points.find(pt->_current->_HTMId);
			this->_points.erase(it);
		}
		old->_current = pt->_current->_children[indexOld];
		this->_pointList.push(pt);
		this->_pointList.push(old);
		pt->_current = pt->_current->_children[indexCurrent];
    }
	else if (pt->_current->_nbChildObject == 0)
    {
		pt->_current->_info = pt;
		this->_points[pt->_current->_HTMId] = pt;
		pt->_current->_nbChildObject = 1;
    }
}

inline std::pair<double, double>       ICoDF_HTM::HTM::CalcCoordPoint(std::pair<double, double>& a, std::pair<double, double>& b)
{
	std::pair<double, double>	result;
	
	result.first = a.first - b.first;
	result.second = a.second - b.second;
	return result;
}

bool ICoDF_HTM::HTM::SelectRootTrixel(PointInfo_t* pt)
{
    Eigen::Vector3d p = pt->_position;
    
	for (int i = 0; i < 8; ++i)
    {
		Eigen::Vector3d* v = this->_octahedron->_rootTrixels[i]->_vertices;
		
		if (v[0].cross(v[1]).dot(p) > 0 &&
			v[1].cross(v[2]).dot(p) > 0 &&
			v[2].cross(v[0]).dot(p) > 0)
		{
			pt->_current = this->_octahedron->_rootTrixels[i];
			return true;
		}
    }
	return false;
}

/// TwoPointsCorrelation
unsigned int ICoDF_HTM::HTM::TwoPointsCorrelation(double& radius, double& delta)
{
	unsigned int nbPairs = 0;
	std::map<std::string, PointInfo_t*>::iterator it;
	
	double infLimit = radius - delta;
	if (infLimit < 0) infLimit = 0;
	double supLimit = radius + delta;
	
    for (std::pair<std::string, PointInfo_t*> const & point : this->_points)
    {
		PointInfo_t* pt = point.second;
		if (IsCorrectRA(pt->_ra) && IsCorrectDEC(pt->_dec))
		{
			Eigen::Vector3d p = pt->_position;
			
			std::queue<trixel_t*> workingList;
			
			for (unsigned int i = 0; i < 4; ++i)
			{
				unsigned short int infInside = 0;
				unsigned short int supInside = 0;
				if (this->_octahedron->_rootTrixels[i] != NULL)
				{
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[0]) > infLimit)
						++infInside;
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[1]) > infLimit)
						++infInside;
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[2]) > infLimit)
						infInside++;
					
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[0]) > supLimit)
						++supInside;
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[1]) > supLimit)
						++supInside;
					if (p.dot(this->_octahedron->_rootTrixels[i]->_vertices[2]) > supLimit)
						supInside++;
				}
				if (supInside == 3 && infInside == 0)
                    nbPairs += this->_octahedron->_rootTrixels[i]->_nbChildObject;
				if ((supInside == 3 && infInside > 0) || supInside > 0)
					workingList.push(this->_octahedron->_rootTrixels[i]);
				else
				{
					Eigen::Vector3d tmpVec1 = _octahedron->_rootTrixels[i]->_vertices[1] - _octahedron->_rootTrixels[i]->_vertices[0];
					Eigen::Vector3d tmpVec2 = _octahedron->_rootTrixels[i]->_vertices[2] - _octahedron->_rootTrixels[i]->_vertices[1];
					Eigen::Vector3d tmpVec3 = tmpVec1.cross(tmpVec2);
					Eigen::Vector3d trixelBoundary = tmpVec3 / tmpVec3.norm();
					
					double theta = acos(trixelBoundary.dot(p) / (trixelBoundary.norm() * p.norm()));
					double phi1 = acos(trixelBoundary.dot(Eigen::Vector3d(1,0,0)) / (trixelBoundary.norm()));
					double phi2 = acos(p.dot(Eigen::Vector3d(1,0,0)) / p.norm());
					if (theta < phi1 + phi2)
					{
						if (!(_octahedron->_rootTrixels[i]->_vertices[0].cross(_octahedron->_rootTrixels[i]->_vertices[1]).dot(p) < 0 &&
							  _octahedron->_rootTrixels[i]->_vertices[1].cross(_octahedron->_rootTrixels[i]->_vertices[2]).dot(p) < 0 &&
							  _octahedron->_rootTrixels[i]->_vertices[2].cross(_octahedron->_rootTrixels[i]->_vertices[0]).dot(p)))
						{
                            nbPairs += 1;
						}
					}
				}
			}
			while (workingList.size() > 0)
			{
				trixel_t* tmp = workingList.front();
				workingList.pop();
				unsigned short int infInside = 0;
				unsigned short int supInside = 0;
				if (p.dot(tmp->_vertices[0]) > infLimit)
					++infInside;
				if (p.dot(tmp->_vertices[2]) > infLimit)
					++infInside;
				if (p.dot(tmp->_vertices[1]) > infLimit)
					++infInside;
				
				if (p.dot(tmp->_vertices[0]) > supLimit)
					++supInside;
				if (p.dot(tmp->_vertices[2]) > supLimit)
					++supInside;
				if (p.dot(tmp->_vertices[1]) > supLimit)
					++supInside;
				
				if (supInside == 3 && infInside == 0)
                    nbPairs += tmp->_nbChildObject;
				else if ((supInside == 3 && infInside > 0)
						 || supInside > 0)
				{
					if (tmp->_children != NULL)
					{
						for (unsigned int i = 0; i < 4; ++i)
							if (tmp->_children[i] != NULL)
								workingList.push(tmp->_children[i]);
					}
					else
                        nbPairs += 1;
				}
				else
				{
					Eigen::Vector3d tmpVec1 = tmp->_vertices[1] - tmp->_vertices[0];
					Eigen::Vector3d tmpVec2 = tmp->_vertices[2] - tmp->_vertices[1];
					Eigen::Vector3d tmpVec3 = tmpVec1.cross(tmpVec2);
					Eigen::Vector3d trixelBoundary = tmpVec3 / tmpVec3.norm();
					
					double theta = acos(trixelBoundary.dot(p) / (trixelBoundary.norm() * p.norm()));
					double phi1 = acos(trixelBoundary.dot(Eigen::Vector3d(1,0,0)) / (trixelBoundary.norm()));
					double phi2 = acos(p.dot(Eigen::Vector3d(1,0,0)) / p.norm());
					if (theta < phi1 + phi2)
					{
						if (!(tmp->_vertices[0].cross(tmp->_vertices[1]).dot(p) < 0 &&
							  tmp->_vertices[1].cross(tmp->_vertices[2]).dot(p) < 0 &&
							  tmp->_vertices[2].cross(tmp->_vertices[0]).dot(p)))
						{
                            nbPairs += 1;
						}
					}
				}
			}
		}
    }
	return nbPairs;
}

void	ICoDF_HTM::HTM::CreateOctahedron(void)
{
	this->_octahedron = new Octahedron_t;
	this->_octahedron->_rootTrixels = new trixel_t*[8];
	Eigen::Vector3d v0( 0,  0,  1);
	Eigen::Vector3d v1( 1,  0,  0);
	Eigen::Vector3d v2( 0,  1,  1);
	Eigen::Vector3d v3(-1,  0,  0);
	Eigen::Vector3d v4( 0, -1,  0);
	Eigen::Vector3d v5( 0,  0, -1);
	
	this->_octahedron->_rootTrixels[0] = CreateRootTrixel(std::string("S0"));
	this->_octahedron->_rootTrixels[0]->_vertices[0] = v1;
	this->_octahedron->_rootTrixels[0]->_vertices[1] = v5;
	this->_octahedron->_rootTrixels[0]->_vertices[2] = v2;
	this->_octahedron->_rootTrixels[1] = CreateRootTrixel(std::string("S1"));
	this->_octahedron->_rootTrixels[1]->_vertices[0] = v2;
	this->_octahedron->_rootTrixels[1]->_vertices[1] = v5;
	this->_octahedron->_rootTrixels[1]->_vertices[2] = v3;
	this->_octahedron->_rootTrixels[2] = CreateRootTrixel(std::string("S2"));
	this->_octahedron->_rootTrixels[2]->_vertices[0] = v3;
	this->_octahedron->_rootTrixels[2]->_vertices[1] = v5;
	this->_octahedron->_rootTrixels[2]->_vertices[2] = v4;
	this->_octahedron->_rootTrixels[3] = CreateRootTrixel(std::string("S3"));
	this->_octahedron->_rootTrixels[3]->_vertices[0] = v4;
	this->_octahedron->_rootTrixels[3]->_vertices[1] = v5;
	this->_octahedron->_rootTrixels[3]->_vertices[2] = v1;
	this->_octahedron->_rootTrixels[4] = CreateRootTrixel(std::string("N0"));
	this->_octahedron->_rootTrixels[4]->_vertices[0] = v1;
	this->_octahedron->_rootTrixels[4]->_vertices[1] = v0;
	this->_octahedron->_rootTrixels[4]->_vertices[2] = v4;
	this->_octahedron->_rootTrixels[5] = CreateRootTrixel(std::string("N1"));
	this->_octahedron->_rootTrixels[5]->_vertices[0] = v4;
	this->_octahedron->_rootTrixels[5]->_vertices[1] = v0;
	this->_octahedron->_rootTrixels[5]->_vertices[2] = v3;
	this->_octahedron->_rootTrixels[6] = CreateRootTrixel(std::string("N2"));
	this->_octahedron->_rootTrixels[6]->_vertices[0] = v3;
	this->_octahedron->_rootTrixels[6]->_vertices[1] = v0;
	this->_octahedron->_rootTrixels[6]->_vertices[2] = v2;
	this->_octahedron->_rootTrixels[7] = CreateRootTrixel(std::string("N3"));
	this->_octahedron->_rootTrixels[7]->_vertices[0] = v2;
	this->_octahedron->_rootTrixels[7]->_vertices[1] = v0;
	this->_octahedron->_rootTrixels[7]->_vertices[2] = v1;
}

void	ICoDF_HTM::HTM::Display(trixel_t* current, std::ofstream& fstream)
{
	if (current != NULL)
    {
		if (current->_children != NULL)
		{
			for (auto i = 0; i < 4; ++i)
			{
				if (current->_children[i] != NULL)
				{
					Display(current->_children[i], fstream);
					if (current->_children[0])
					{
						if (this->_points[current->_children[0]->_HTMId])
						{
							PointInfo_t* info = this->_points[current->_children[0]->_HTMId];
							fstream << "Item stored at trixel : " << current->_children[0]->_HTMId << " with right ascension and declinaison at " << info->_ra << " " << info->_dec << std::endl;
						}
					}
					else if (current->_children[1])
					{
						if (this->_points[current->_children[1]->_HTMId])
						{
							PointInfo_t* info = this->_points[current->_children[1]->_HTMId];
							fstream << "Item stored at trixel : " << current->_children[1]->_HTMId << " with right ascension and declinaison at " << info->_ra << " " << info->_dec << std::endl;
						}
					}
					else if (current->_children[2])
					{
						if (this->_points[current->_children[2]->_HTMId])
						{
							PointInfo_t* info = this->_points[current->_children[2]->_HTMId];
							fstream << "Item stored at trixel : " << current->_children[2]->_HTMId << " with right ascension and declinaison at " << info->_ra << " " << info->_dec << std::endl;
						}
					}
					else if (current->_children[3])
					{
						if (this->_points[current->_children[3]->_HTMId])
						{
							PointInfo_t* info = this->_points[current->_children[3]->_HTMId];
							fstream << "Item stored at trixel : " << current->_children[3]->_HTMId << " with right ascension and declinaison at " << info->_ra << " " << info->_dec << std::endl;
						}
					}
				}
			}
		}
    }
}

void	ICoDF_HTM::HTM::FreeAllTrixels(trixel_t* current)
{
	if (current != NULL)
    {
		if (current->_children != NULL)
		{
			for (auto i = 0; i < 4; ++i)
			{
				if (current->_children[i] != NULL)
				{
					FreeAllTrixels(current->_children[i]);
					delete current->_children[i];
				}
            }
			delete[] current->_children;
                
		}
    }
}

void ICoDF_HTM::HTM::LogIntoFile(std::string const & parFilename)
{
    std::ofstream fstream;
	fstream.open(parFilename);
	for (auto i = 0; i < 8; ++i)
		this->Display(this->_octahedron->_rootTrixels[i], fstream);
	fstream.close();
}

void	ICoDF_HTM::HTM::DeleteOctahedron(void)
{
	for (auto i = 0; i < 8; ++i)
		this->FreeAllTrixels(this->_octahedron->_rootTrixels[i]);
	for (auto it = this->_points.begin(); it != this->_points.end(); ++it)
		delete it->second;
	for (auto i = 0; i < 8; ++i)
		delete this->_octahedron->_rootTrixels[i];
	this->_points.clear();
	delete this->_octahedron;
    this->_octahedron = 0;
}

/// Create the HTM
ICoDF_HTM::HTM::HTM()
{
    //LS_ADDMSG(LogService::NOTICE, "HTM", "HTM core created");
    _points.reserve(10000);
}

ICoDF_HTM::HTM::~HTM()
{
	//LS_ADDMSG(LogService::NOTICE, "HTM", "HTM core deleted");
}

// Cedric: V2

void ICoDF_HTM::HTM::UniformNumberGenerator(unsigned int const nbObject, double const raMin, double const raMax, double const decMin, double const decMax)
{
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed1);
	std::uniform_real_distribution<double> unif1(raMin, raMax);
	std::uniform_real_distribution<double> unif2(decMin, decMax);
	for (unsigned int i = 0; i < nbObject; ++i)
    {
		const double ra = unif1(gen);
		const double dec = unif2(gen);
		this->AddPoint(ra, dec);
    }
}

void ICoDF_HTM::HTM::GeneratePoint(std::vector<std::pair<double, double>> const & parPointsDefinition)
{
    for (std::pair<double, double> const & pointDefinition : parPointsDefinition)
    {
        this->AddPoint(pointDefinition.first, pointDefinition.second);
    }
}