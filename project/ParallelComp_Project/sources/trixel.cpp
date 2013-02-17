#include "../includes/GenericMemoryPool.hpp"
#include "../includes/trixel.hpp"
/*
void* trixel_t::operator new(size_t size)
{
    return GenericMemoryPool<trixel_t>::GetInstance()->Retain();
}

void trixel_t::operator delete(void* ptr)
{
    GenericMemoryPool<trixel_t>::GetInstance()->Release(ptr);
}
*/

// ------------------------------------------------------------------------------
// CREATETRIXELCHILDREN
trixel_t** ICoDF_HTM::CreateTrixelChildren(trixel_t *parent)
{
	if (parent->_children != NULL)
    {
		for (int i = 0; i < 4; ++i)
		{
			if (parent->_children[i] != NULL)
			{
				LS_ADDMSG(LogService::NOTICE, "ICoDF::CreateTrixelChildren", "Trixel already have child(ren)");
			}
		}
    }
	else
    {
		parent->_children = new trixel_t*[4];
		for (int i = 0; i <  4; ++i)
		{
			parent->_children[i] = NULL;
		}
    }
	return parent->_children;
}

// -------------------------------------------------------------------------------
void ICoDF_HTM::ComputeTrixelMidpoints(trixel_t* trixel, Vector3d* outMidPoint)
{
 
    if (!trixel->_midPointsComputed)
    {
        Vector3d tmp;
        
        tmp = trixel->_vertices[1] + trixel->_vertices[2];
        trixel->_midPoints[0] = tmp / tmp.norm();
        tmp = trixel->_vertices[0] + trixel->_vertices[2];
        trixel->_midPoints[1] = tmp / tmp.norm();
        tmp = trixel->_vertices[0] + trixel->_vertices[1];
        trixel->_midPoints[2] = tmp / tmp.norm();
        trixel->_midPointsComputed = true;
    }
    
    outMidPoint[0] = trixel->_midPoints[0];
    outMidPoint[1] = trixel->_midPoints[1];
    outMidPoint[2] = trixel->_midPoints[2];
}

// -------------------------------------------------------------------------------
// CREATEROOTTRIXEL
trixel_t* ICoDF_HTM::CreateRootTrixel(std::string HTMId)
{
	trixel_t* trixel = new trixel_t();
	InitTrixel(trixel);
	trixel->_HTMId = HTMId;
	return trixel;
}

// -------------------------------------------------------------------------------
// CREATETRIXELCHILD
// Check if trixel children structure already exists
// Assert for given <index>
// Check if select child does not exist
//   Create new vertices on side midpoints
//   Set vertices that defines the new subtrixel
// else display a message
// return the pointer
trixel_t* ICoDF_HTM::CreateTrixelChild(trixel_t* parent, unsigned short int& index)
{
	if (parent->_children == NULL)
    {
		LS_ADDMSG(LogService::NOTICE, "ICoDF::CreateTrixelChild", "Trixel as no container for children");
		CreateTrixelChildren(parent);
    }
	
	assert(index < 4);
	
	if (parent->_children[index] == NULL)
    {
		std::stringstream tmp;
		parent->_children[index] = new trixel_t();
		InitTrixel(parent->_children[index]);
		tmp.str("");
		tmp << parent->_HTMId << index;
		parent->_children[index]->_HTMId = tmp.str();
        Vector3d midPoints[3];
		ComputeTrixelMidpoints(parent, midPoints);
		
		switch (index)
		{
			case 0:
				parent->_children[index]->_vertices[0] = parent->_vertices[0];
				parent->_children[index]->_vertices[1] = midPoints[2];
				parent->_children[index]->_vertices[2] = midPoints[1];
				break;
			case 1:
				parent->_children[index]->_vertices[0] = parent->_vertices[1];
				parent->_children[index]->_vertices[1] = midPoints[0];
				parent->_children[index]->_vertices[2] = midPoints[2];
				break;
			case 2:
				parent->_children[index]->_vertices[0] = parent->_vertices[2];
				parent->_children[index]->_vertices[1] = midPoints[1];
				parent->_children[index]->_vertices[2] = midPoints[0];
				break;
			case 3:
				parent->_children[index]->_vertices[0] = midPoints[0];
				parent->_children[index]->_vertices[1] = midPoints[1];
				parent->_children[index]->_vertices[2] = midPoints[2];
				break;
			default:
				LS_ADDMSG(LogService::FATAL, "ICoDF::CreateTrixelChild", "Given <index> is out of bound");
				return NULL;
		}
    }
	else
    {
		std::stringstream tmp;
		tmp << "SubTrixel [" << parent->_HTMId << index << "] already exists";
		LS_ADDMSG(LogService::NOTICE, "ICoDF::CreateTrixelChild", tmp.str());
    }
	
	return parent->_children[index];
}

// -------------------------------------------------------------------
// CLEARTRIXELCHILDREN
void ICoDF_HTM::ClearTrixelChildren(trixel_t *parent)
{
	if (parent->_children != NULL)
    {
		for (short int i = 0; i < 4; ++i)
		{
			if (parent->_children[i] != NULL)
			{
				ClearTrixel(parent->_children[i]);
				parent->_children[i] = NULL;
			}
		}
		delete [] parent->_children;
		parent->_children = NULL;
    }
}

// ---------------------------------------------------------------------
// CLEARTRIXELCHILDREN
void ICoDF_HTM::ClearTrixel(trixel_t *trixel)
{
	ClearTrixelChildren(trixel);
}

// --------------------------------------------------------------------
// GETINDEX (vector version)
unsigned short int ICoDF_HTM::GetIndex(trixel_t* trixel, Vector3d& p)
{
	if (trixel != NULL && NULL != trixel->_vertices)
    {
		unsigned short int index = (unsigned short int)~0;
		
		Vector3d* v = trixel->_vertices;
		Vector3d w[3];
        ComputeTrixelMidpoints(trixel, w);
        
        // HERE
		if (v[0].cross(w[2]).dot(p) > 0 &&
			w[2].cross(w[1]).dot(p) > 0 &&
			w[1].cross(v[0]).dot(p) > 0)
			index = 0;
		else if (v[1].cross(w[0]).dot(p) > 0 &&
				 w[0].cross(w[2]).dot(p) > 0 &&
				 w[2].cross(v[1]).dot(p) > 0)
			index = 1;
		else if (v[2].cross(w[1]).dot(p) > 0 &&
				 w[1].cross(w[0]).dot(p) > 0 &&
				 w[0].cross(v[2]).dot(p) > 0)
			index = 2;
		else if (w[0].cross(w[1]).dot(p) > 0 &&
				 w[1].cross(w[2]).dot(p) > 0 &&
				 w[2].cross(w[0]).dot(p) > 0)
			index = 3;
		
        return index;
    }
	else
    {
		LS_ADDMSG(LogService::WARNING, "ICoDF_HTM::GetIndex", "Given <trixel> or its vertices has a NULL value");
		return (unsigned short int)~0;
    }
}

// --------------------------------------------------------------------
// GETINDEX (PointInfo version)
unsigned short int ICoDF_HTM::GetIndex(trixel_t* trixel, PointInfo_t* pointInfo)
{
    double ra = pointInfo->_ra;
    double dec = pointInfo->_dec;
    
    if (IsCorrectRA(ra) && IsCorrectDEC(dec))
    {
		unsigned short int index = GetIndex(trixel, pointInfo->_position);
        return index;
    }
	else
    {
		std::stringstream tmp;
		tmp << "Given <ra> [" << ra << "] or <dec> [" << dec << "] is out of bounds";
		LS_ADDMSG(LogService::WARNING, "ICoDF_HTM", tmp.str());
    }
	return (unsigned short int)~0;
}

// --------------------------------------------------------------------
// INITTRIXEL
void ICoDF_HTM::InitTrixel(trixel_t* trixel)
{
	if (trixel == NULL)
    {
		LS_ADDMSG(LogService::WARNING, "ICoDF_HTM::InitTrixel", "Given <trixel> has a NULL value");
    }
	else
    {
		trixel->_children = NULL;
		trixel->_reverse = false;
		trixel->_HTMId = "";
		trixel->_nbChildObject = 0;
		trixel->_info = NULL;
    }
}

// --------------------------------------------------------------------
// ISCORRECTRA
bool ICoDF_HTM::IsCorrectRA(double const ra)
{
	if (ra >= 0 && ra < 360)
		return true;
	return false;
}

// --------------------------------------------------------------------
// ISCORRECTDEC
bool ICoDF_HTM::IsCorrectDEC(double const dec)
{
	if (dec > -90 && dec <= 90)
		return true;
	return false;
}
