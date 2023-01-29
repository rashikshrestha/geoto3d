#include "ncvoxel.h"
#include "ncqtprogram.h"

void ncVoxTreeBase::clear() {
    pmin        = ncc::v3ll(0,0,0);
    maxlevel    = 0;
    minlevel    = 0;
    maxcarve    = 0;
    voxels      = ncVectorArray();
}

ncc::RTd ncVoxTreeBase::getTransform() {
    return ncc::RTd::translate(0.001*pmin.x,0.001*pmin.y,0.001*pmin.z);
}

void ncVoxTreeBase::print() {

    fprintf(stderr,"filename    = %s\n",filename.c_str());
    fprintf(stderr,"data_mask   = %08X\n",voxels.data_mask);
    fprintf(stderr,"pmin        = %lld %lld %lld\n",pmin.x,pmin.y,pmin.z);
    fprintf(stderr,"maxlevel    = %d\n",maxlevel);
    fprintf(stderr,"minlevel    = %d\n",minlevel);
}

void ncVoxTreeBase::load(std::ifstream& file) {
    ncVoxTreeBase::clear();

    file.read((char *)&pmin.x   , sizeof(ncc::v3ll      ));
    file.read((char *)&maxlevel , sizeof(int            ));
    file.read((char *)&minlevel , sizeof(int            ));
    file.read((char *)&maxcarve , sizeof(unsigned int   ));

    voxels.load(file);
}

void ncVoxTreeBase::save(std::ofstream& file , bool optimize) {
    file.write((char *)&pmin.x              , sizeof(ncc::v3ll      ));
    file.write((char *)&maxlevel	        , sizeof(int            ));
    file.write((char *)&minlevel	        , sizeof(int            ));
    file.write((char *)&maxcarve            , sizeof(unsigned int   ));

    if (optimize)
        optimizeChilds();
    voxels.save(file);
}

void ncVoxTreeBase::optimizeChilds() {

    ncVectorArray vout(voxels.data_mask);
    ncVectorArray::copy(vout,0,voxels,0);

    unsigned int index = 0;
    while (index < vout.size())
    {
        if (vout.getValue<NC_CHILD>(index))
        {
            for (unsigned int i=0; i<8; ++i)
            {
                unsigned int k = vout.newItem();
                ncVectorArray::copy(vout,k,voxels,vout.getValue<NC_CHILD>(index)+i);
            }
            vout.setValue<NC_CHILD>(index , vout.size() - 8);
        }
        index++;
    }

    voxels = vout;
}

ncVoxTreeIterator ncVoxTreeBase::getIterator(ncc::v4i pos) {
    ncVoxTreeIterator vp;
    vp.pos = pos;
    getIterator(vp,maxlevel);
    return vp;
}

void ncVoxTreeBase::getIterator(ncVoxTreeIterator & vp , int level) {
    if (level > vp.pos.w)
    {
        if (voxels.getValue<NC_CHILD>(vp.id))
        {
            level--;
            int mask    = 1<<level;
            int c = (((vp.pos.z&mask)>>level)<<2) | (((vp.pos.y&mask)>>level)<<1) | ((vp.pos.x&mask)>>level);
            vp.id = voxels.getValue<NC_CHILD>(vp.id)+c;
            getIterator(vp,level);
        }
        else
        {
            int mask    = ~((1<<level)-1);
            vp.pos.x&=mask;
            vp.pos.y&=mask;
            vp.pos.z&=mask;
            vp.pos.w =level;
        }
    }
}

unsigned int ncVoxTreeBase::split(unsigned int id , int level) {

    unsigned int cid = voxels.getValue<NC_CHILD>(id);
    if (!cid)
    {
        cid = voxels.newItem();
        for (unsigned int i=1; i<8; ++i)
            voxels.newItem();

        voxels.setValue<NC_CHILD>(id , cid);
        for (unsigned int i=0; i<8; ++i)
        {
            ncVectorArray::copy(voxels,cid+i,voxels,id);
            voxels.setValue<NC_CHILD>(cid+i , 0);
        }
    }
    return cid;
}

ncVoxTreeIterator ncVoxTreeBase::subDivideTo(ncc::v4i pos) {
    ncVoxTreeIterator vp(0,pos);
    subDivideTo(vp,maxlevel);
    return vp;
}

void ncVoxTreeBase::subDivideTo(ncVoxTreeIterator& vp , int level) {
    if (level>vp.pos.w)
    {
        unsigned int cid = split(vp.id , vp.pos.w);

        level--;
        int mask    = 1<<level;
        int c = (((vp.pos.z&mask)>>level)<<2) | (((vp.pos.y&mask)>>level)<<1) | ((vp.pos.x&mask)>>level);
        vp.id = cid + c;
        subDivideTo(vp , level);
    }
}








ncSubVoxTree::ncSubVoxTree() {
}

ncSubVoxTree::ncSubVoxTree(std::string fname) {
    //fprintf(stderr,"+ncSubVoxTree::load(%s)\n",fname.c_str());
    load(fname);
    //fprintf(stderr,"-ncSubVoxTree::load(%s)\n",fname.c_str());
}

ncSubVoxTree::ncSubVoxTree(std::string fname , unsigned int data_mask_) {
    ncSubVoxTree::clear();
    filename    = fname;
    voxels      = ncVectorArray(data_mask_);
}

ncSubVoxTree::ncSubVoxTree(std::string fname , unsigned int data_mask_ , const ncSubVoxTree& vtree ) {
    ncSubVoxTree::clear();

    filename    = fname;

    pmin        = vtree.pmin;
    maxlevel    = vtree.maxlevel;
    minlevel    = vtree.minlevel;
    maxcarve    = vtree.maxcarve;
    voxels      = vtree.voxels;

    setDataMask(data_mask_);

}

ncSubVoxTree::ncSubVoxTree(std::string fname_ , unsigned int data_mask_ , ncc::v3ll pmin_ , int maxlevel_ , int minlevel_ , unsigned int maxcarve_) {
    ncSubVoxTree::clear();

    filename    = fname_;
    pmin        = pmin_;
    maxlevel    = maxlevel_;
    minlevel    = minlevel_;
    maxcarve    = maxcarve_;
    voxels      = ncVectorArray(data_mask_);
}

ncSubVoxTree::~ncSubVoxTree() {
    ncSubVoxTree::clear();
}

void ncSubVoxTree::clear() {

    ncVoxTreeBase::clear();

    memory_pool.clear();
    voxels  = ncVectorArray();
}

void ncSubVoxTree::print() {

    fprintf(stderr,"\n");
    fprintf(stderr,"ncSubVoxTree Info:\n");
    fprintf(stderr,"filename    = %s\n",filename.c_str());
    fprintf(stderr,"pmin        = %lld %lld %lld\n",pmin.x,pmin.y,pmin.z);
    fprintf(stderr,"maxlevel    = %d\n",maxlevel);
    fprintf(stderr,"minlevel    = %d\n",minlevel);
    fprintf(stderr,"#voxels     = %d\n",voxels.size());
    fprintf(stderr,"\n");
}

void ncSubVoxTree::load(std::string fname) {
    //fprintf(stderr,"+ncSubVoxTree::load(%s)\n",fname.c_str());
    if (!fname.empty())
    {
        filename = fname;
        std::ifstream file(fname.c_str(), std::ios::in | std::ios::binary);
        ncVoxTreeBase::load(file);
        file.close();
    }
    //fprintf(stderr,"-ncSubVoxTree::load(%s)\n",fname.c_str());
}

void ncSubVoxTree::save() {
    //fprintf(stderr,"+ncSubVoxTree::save(%s)\n",filename.c_str());
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    ncVoxTreeBase::save(file,true);
    file.close();
    //fprintf(stderr,"-ncSubVoxTree::save(%s)\n",filename.c_str());
}

void ncSubVoxTree::optimizeCarve( unsigned int max_carve , int min_level) {

    //fprintf(stderr,"Optimizing [%d]...",max_carve);

    maxcarve = max_carve;
    minlevel = min_level;

    optimizeCarveRecursive(0,maxlevel);
    //optimizeChilds();

    //fprintf(stderr,"done\n");
}

int  ncSubVoxTree::optimizeCarveRecursive(unsigned int id , int cur_level) {

    if (voxels.getValue<NC_CARVE>(id)>=maxcarve)
    {
        voxels.setValue<NC_RGBM>(id , 0,0,0,0x00);
        voxels.setValue<NC_CHILD>(id ,  0);
        return -1;
    }
    else if ((cur_level>minlevel) && voxels.getValue<NC_CHILD>(id))
    {
        voxels.setValue<NC_RGBM>(id , 0,0,0,0x00);

        unsigned int count = 0;
        unsigned int mask = 0;
        for (unsigned int i=0;i<8;++i)
        {
            int tmp = optimizeCarveRecursive(voxels.getValue<NC_CHILD>(id)+i,cur_level-1);
            count+= tmp;
            if (tmp != -1)
               mask |= 1<<i;
        }
        voxels.setValue<NC_RGBM>(id , 0,0,0,mask);
        if (count == +8)
        {
            voxels.setValue<NC_RGBM>(id , 0,0,0,0xFF);
            voxels.setValue<NC_CHILD>(id ,  0);
            return +1;
        }
        else if (count == -8)
        {
            voxels.setValue<NC_RGBM>(id , 0,0,0,0x00);
            voxels.setValue<NC_CHILD>(id ,  0);
            return -1;
        }
        return 0;
    }
    else
    {
        voxels.setValue<NC_RGBM>(id , 0,0,0,0xFF);
        voxels.setValue<NC_CHILD>(id ,  0);
        return +1;
    }

}

void ncSubVoxTree::subDivide() {
    ncVoxTreeIterator vp(0,ncc::v4i(0,0,0,maxlevel));
    subDivide(vp);
}

void ncSubVoxTree::subDivide(ncVoxTreeIterator vp) {
    //fprintf(stderr,"+subDivide(ncVoxTreeIterator(%d , ncc::v4i(%d,%d,%d,%d)))\n",vp.id,vp.pos.x,vp.pos.y,vp.pos.z,vp.pos.w);
    if (vp.pos.w>minlevel)
    {
        if (voxels.getValue<NC_CHILD>(vp.id))
        {
            int offset = 1<<(vp.pos.w-1);
            for (unsigned int i=0; i<8; ++i)
                subDivide(ncVoxTreeIterator(voxels.getValue<NC_CHILD>(vp.id)+i,ncc::v4i(vp.pos.x + ( i&0x1 ? offset : 0),vp.pos.y + ( i&0x2 ? offset : 0),vp.pos.z + ( i&0x4 ? offset : 0),vp.pos.w-1)));
        }
        else if (voxels.getValue<NC_RGBM>(vp.id).mask == 0x00)
        {
            int offset1 = 1<<vp.pos.w;

            if (vp.pos.x-offset1 >= 0)
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x-offset1          ,vp.pos.y          ,vp.pos.z          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,POS_X);
            }
            if (vp.pos.x+offset1 < (1<<maxlevel))
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x+offset1          ,vp.pos.y          ,vp.pos.z          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,NEG_X);
            }
            if (vp.pos.y-offset1 >= 0)
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x          ,vp.pos.y-offset1          ,vp.pos.z          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,POS_Y);
            }
            if (vp.pos.y+offset1 < (1<<maxlevel))
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x          ,vp.pos.y+offset1          ,vp.pos.z          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,NEG_Y);
            }
            if (vp.pos.z-offset1 >= 0)
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x          ,vp.pos.y          ,vp.pos.z-offset1          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,POS_Z);
            }
            if (vp.pos.z+offset1 < (1<<maxlevel))
            {
                ncVoxTreeIterator nb = subDivideTo(ncc::v4i(vp.pos.x          ,vp.pos.y          ,vp.pos.z+offset1          ,vp.pos.w));
                splitSurface(nb.id,nb.pos.w,NEG_Z);
            }
        }
    }
    //fprintf(stderr,"-subDivide(ncVoxTreeIterator vp)\n");
}

void ncSubVoxTree::splitSurface(unsigned int id , int level, unsigned int locmask) {

    if (voxels.getValue<NC_RGBM>(id).mask != 0)
    {
        if (level>minlevel)
        {
            //fprintf(stderr,"level = %d\n",level);
            if (!voxels.getValue<NC_CHILD>(id))
            {
                split(id , level);
                //fprintf(stderr,".");
            }

            level--;
            if (locmask==NEG_X)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+0 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+2 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+4 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+6 , level , locmask);
            }
            if (locmask==POS_X)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+1 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+3 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+5 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+7 , level , locmask);
            }
            if (locmask==NEG_Y)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+0 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+1 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+4 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+5 , level , locmask);
            }
            if (locmask==POS_Y)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+2 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+3 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+6 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+7 , level , locmask);
            }
            if (locmask==NEG_Z)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+0 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+1 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+2 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+3 , level , locmask);
            }
            if (locmask==POS_Z)
            {
                splitSurface(voxels.getValue<NC_CHILD>(id)+4 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+5 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+6 , level , locmask);
                splitSurface(voxels.getValue<NC_CHILD>(id)+7 , level , locmask);
            }
        }
    }

    //fprintf(stderr,"+subDivideTo(ncVoxTreeIterator(%d , ncc::v4i(%d,%d,%d,%d)) , %d)\n",vp.vox,vp.pos.x,vp.pos.y,vp.pos.z,vp.pos.w,level);
    //fprintf(stderr,"-subDivideTo(ncVoxTreeIterator vp , int level)\n");
}

void ncSubVoxTree::createColors() {
    for (unsigned int i=0;i<voxels.size();++i)
    {
        ncColor tmp_color = voxels.getValue<NC_COLOR>(i);
        ncRGBM  tmp_rgbm  = voxels.getValue<NC_RGBM>(i);

        if (tmp_color.a > 0)
            voxels.setValue<NC_RGBM>(i, (tmp_color.r/tmp_color.a)&0xFF , (tmp_color.g/tmp_color.a)&0xFF , (tmp_color.b/tmp_color.a)&0xFF , tmp_rgbm.mask);
        else
            voxels.setValue<NC_RGBM>(i,                              0 ,                              0 ,                              0 , tmp_rgbm.mask);
    }

    colorUp();
    colorDown();
}

void ncSubVoxTree::colorUp() {
    colorUp(0,maxlevel);
}

ncColor ncSubVoxTree::colorUp(unsigned int id, int level) {

    ncColor color(0,0,0,0);

    if (voxels.getValue<NC_CHILD>(id))
    {
        for (unsigned int i=0;i<8;++i)
            color += colorUp(voxels.getValue<NC_CHILD>(id)+i , level-1);

        if ((color.a>0) && (level<=minlevel+2))
            voxels.setValue<NC_RGBM>(id ,  color.r/color.a , color.g/color.a , color.b/color.a , voxels.getValue<NC_RGBM>(id).mask);
        else
            voxels.setValue<NC_RGBM>(id ,                0 ,               0 ,               0 , voxels.getValue<NC_RGBM>(id).mask);
    }
    else
    {
        ncRGBM tmp = voxels.getValue<NC_RGBM>(id);

        if ((tmp.r==0) && (tmp.g==0) && (tmp.b==0))
            color = ncColor(0,0,0,0);
        else
            color = ncColor(tmp.r,tmp.g,tmp.b,1);
    }
    return color;
}

void ncSubVoxTree::colorDown(ncColor color, unsigned int id) {

    ncRGBM tmp = voxels.getValue<NC_RGBM>(id);

    if ((id==0) || ((tmp.r!=0) || (tmp.g!=0) || (tmp.b!=0)))
    {
        color.r =  tmp.r;
        color.g =  tmp.g;
        color.b =  tmp.b;
        color.a =  1;
    }
    else
        voxels.setValue<NC_RGBM>(id ,  color.r/color.a , color.g/color.a , color.b/color.a , voxels.getValue<NC_RGBM>(id).mask);

    if (voxels.getValue<NC_CHILD>(id))
        for (unsigned int i=0;i<8;++i)
            colorDown(color , voxels.getValue<NC_CHILD>(id)+i);
}

void ncSubVoxTree::pushID(unsigned int id) {
    memory_pool.push_back(id);
}

unsigned int ncSubVoxTree::popID() {
    if (memory_pool.size()>0)
    {
        unsigned int id = memory_pool.back();
        memory_pool.pop_back();
        return id;
    }
    else
    {
        voxels.resize(voxels.size()+8);
        return voxels.size()-8;
    }
}

void ncSubVoxTree::clearID(unsigned int id) {
    if (voxels.getValue<NC_CHILD>(id))
    {
        pushID(voxels.getValue<NC_CHILD>(id));
        for (unsigned int i=0;i<8;++i)
            clearID(voxels.getValue<NC_CHILD>(id)+i);
        voxels.setValue<NC_CHILD>(id , 0);
    }
}

void ncSubVoxTree::incCarve(unsigned int id , int level) {
    if (voxels.getValue<NC_CARVE>(id)<maxcarve)
    {
        voxels.getValue<NC_CARVE>(id)++;
        if (voxels.getValue<NC_CHILD>(id))
            for (unsigned int i=0; i<8; ++i)
                incCarve(voxels.getValue<NC_CHILD>(id)+i,level-1);
    }
    else
    {
        clearID(id);
    }
}

unsigned int ncSubVoxTree::split(unsigned int id , int level) {
    unsigned int cid = voxels.getValue<NC_CHILD>(id);
    if (!cid)
    {
        cid = popID();
        voxels.getValue<NC_CHILD>(id) = cid;

        for (unsigned int i=0; i<8; ++i)
        {
            ncVectorArray::copy(voxels,cid+i,voxels,id);
            voxels.setValue<NC_CHILD>(cid+i , 0);
        }
    }
    return cid;
}

void ncSubVoxTree::setDataMask(unsigned int mask) {
    voxels.setDataMask(mask);
}





ncVoxTree::ncVoxTree() {
}

ncVoxTree::ncVoxTree(std::string filename_ , bool create_sublevel_ , bool save_to_disk_ ) {
    load(filename_ , create_sublevel_ , save_to_disk_);
}

ncVoxTree::ncVoxTree(std::string filename_ , bool create_sublevel_ , bool save_to_disk_ , unsigned int data_mask_) {
    ncVoxTree::clear();
    filename        = filename_;
    create_sublevel = create_sublevel_;
    save_to_disk    = save_to_disk_;
    data_mask       = data_mask_;
    voxels          = ncVectorArray(NC_CHILD | NC_PTR);
}

ncVoxTree::ncVoxTree(std::string filename_ , bool create_sublevel_ , bool save_to_disk_ , unsigned int data_mask_ , ncc::v3ll pmin_ , int maxlevel_ , int sublevel_ , int minlevel_ , unsigned int maxcarve_) {
    ncVoxTree::clear();
    filename        = filename_;
    create_sublevel = create_sublevel_;
    save_to_disk    = save_to_disk_;
    data_mask       = data_mask_;
    pmin            = pmin_;
    maxlevel        = maxlevel_;
    sublevel        = sublevel_;
    minlevel        = minlevel_;
    maxcarve        = maxcarve_;
    voxels          = ncVectorArray(NC_CHILD | NC_PTR);
}

ncVoxTree::ncVoxTree(std::string filename_ , bool create_sublevel_ , bool save_to_disk_ , unsigned int data_mask_ , const ncVoxTree& vtree ) {
    filename        = filename_;
    create_sublevel = create_sublevel_;
    save_to_disk    = save_to_disk_;
    data_mask       = data_mask_;

    pmin            = vtree.pmin;
    maxlevel        = vtree.maxlevel;
    sublevel        = vtree.sublevel;
    minlevel        = vtree.minlevel;
    maxcarve        = vtree.maxcarve;
    voxels          = vtree.voxels;
    subs            = vtree.subs;
    LRU             = vtree.LRU;
}

ncVoxTree::~ncVoxTree() {

    save();
    ncVoxTree::clear();
}

void ncVoxTree::clear() {
    ncVoxTreeBase::clear();

    max_resident    = 4*4*4;
    sublevel        = 0;
    LRU.clear();
    subs.clear();
}

/*
void ncVoxTree::addID(unsigned int id_out, unsigned int id_in , ncVectorArray & vin) {

    clearID(id_out);

    ncVectorArray::copy(voxels,id_out,vin,id_in);

    if (vin.getValue<NC_CHILD>(id_in))
    {
        unsigned int cid = popID();
        for (unsigned int i=0;i<8;++i)
            addID(cid+i, vin.getValue<NC_CHILD>(id_in)+i , vin);
        voxels.setValue<NC_CHILD>(id_out, cid);
    }
}
*/

void ncVoxTree::print() {

    fprintf(stderr,"\n");
    fprintf(stderr,"ncVoxTree Info:\n");
    ncVoxTreeBase::print();
    fprintf(stderr,"sublevel    = %d\n",sublevel);
    fprintf(stderr,"\n");
}

void ncVoxTree::load(std::string fname , bool create_sublevel_ , bool save_to_disk_ ) {
    filename        = fname;
    create_sublevel = create_sublevel_;
    save_to_disk    = save_to_disk_;

    ncVoxTree::clear();

    fprintf(stderr,"+ncVoxTree::load(%s)\n",filename.c_str());

    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    file.read((char *)&data_mask , sizeof(unsigned int   ));
    file.read((char *)&sublevel  , sizeof(int            ));
    unsigned int nrsubs;
    file.read((char *)&nrsubs    , sizeof(unsigned int   ));
    subs.resize(nrsubs);
    file.read((char *)&subs[0] , nrsubs*sizeof(ncVoxTreeIterator));

    ncVoxTreeBase::load(file);
    file.close();

    for (unsigned int i=0;i<subs.size();++i)
        subs[i] = getIterator(subs[i].pos);

    //fprintf(stderr,"-ncVoxTree::load(%s)\n",filename.c_str());

}

void ncVoxTree::save() {
    //fprintf(stderr,"+ncVoxTree::save(%s) -> %d\n",filename.c_str(),(voxels.data_mask&NC_COLOR? 1 : 0));

    while(LRU.size()>0)
        saveSubLevel(LRU.front());

    if (save_to_disk)
    {
        std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
        file.write((char *)&data_mask   , sizeof(unsigned int   ));
        file.write((char *)&sublevel    , sizeof(int            ));

        unsigned int nrsubs = subs.size();
        file.write((char *)&nrsubs    , sizeof(unsigned int             ));
        file.write((char *)&subs[0]	  , nrsubs*sizeof(ncVoxTreeIterator ));

        ncVoxTreeBase::save(file , false);
        file.close();
    }

    //fprintf(stderr,"-ncVoxTree::save(%s)\n",filename.c_str());
}

std::string ncVoxTree::getFilename(ncc::v3i vpos) {
    char fname[1024];
    sprintf(fname,"%s.%08X.%08X.%08X.%08X.gvm",filename.c_str(),vpos.x,vpos.y,vpos.z,sublevel);
    return std::string(fname);
}

void ncVoxTree::loadSubLevel(unsigned int index) {
    std::list<unsigned int>::iterator lru_position = LRU.begin();
    while ( (lru_position!=LRU.end()) && (*lru_position != index))
        lru_position++;

    if (lru_position==LRU.end())
    {
        if (LRU.size() >= max_resident)
            saveSubLevel(LRU.back());

        loadSubLevel(subs[index]);
        LRU.push_front(index);
    }
    else
    {
        LRU.erase(lru_position);
        LRU.push_front(index);
    }

}

unsigned int ncVoxTree::addSubLevel(ncc::v3i pos) {
    ncVoxTreeIterator tmp = subDivideTo(ncc::v4i(pos.x,pos.y,pos.z,sublevel));

    for (unsigned int i=0;i<subs.size();++i)
        if (subs[i] == tmp)
        {
            loadSubLevel(i);
            return i;
        }

    //if (create_sublevel)
    {
        if (LRU.size() == max_resident)
            saveSubLevel(LRU.back());

        loadSubLevel(tmp, data_mask , pmin+pos , sublevel , minlevel , maxcarve);
        subs.push_back(tmp);
        LRU.push_front(subs.size()-1);
        return subs.size()-1;
    }

}

void ncVoxTree::saveSubLevel(unsigned int index) {
    std::list<unsigned int>::iterator lru_position = LRU.begin();
    while ( (lru_position!=LRU.end()) && (*lru_position != index))
        lru_position++;

    if (lru_position!=LRU.end())
    {
        saveSubLevel(subs[*lru_position]);
        LRU.erase(lru_position);
    }

}

void ncVoxTree::loadSubLevel(ncVoxTreeIterator it) {
    fprintf(stderr,"+loadSubLevel(%u)\n",it.id);
    voxels.getValue<NC_PTR>(it.id) = new ncSubVoxTree(getFilename(it.pos.xyz()).c_str());
}

void ncVoxTree::loadSubLevel(ncVoxTreeIterator it , unsigned int data_mask , ncc::v3ll pmin , int maxlevel , int minlevel , unsigned int maxcarve) {
    fprintf(stderr,"+addSubLevel(%d,%d,%d)\n",it.pos.x,it.pos.y,it.pos.z);
    voxels.getValue<NC_PTR>(it.id) = new ncSubVoxTree(getFilename(it.pos.xyz()).c_str() , data_mask , pmin , maxlevel , minlevel , maxcarve);
}

void ncVoxTree::saveSubLevel(ncVoxTreeIterator it) {
    fprintf(stderr,"+saveSubLevel(%u)\n",it.id);
    ncSubVoxTree*& vt = (ncSubVoxTree*&)voxels.getValue<NC_PTR>(it.id).value;
    if (save_to_disk)
        vt->save();
    delete vt;
    vt = 0;
}

void ncVoxTree::optimizeCarve( unsigned int max_carve , int min_level) {
    while(LRU.size()>0)
        saveSubLevel(LRU.front());

    for (unsigned int i=0;i<subs.size();++i)
    {
        ncSubVoxTree tmp(getFilename(subs[i].pos.xyz()));
        tmp.optimizeCarve(max_carve,min_level);
        tmp.save();
    }
}

void ncVoxTree::subDivide() {
    while(LRU.size()>0)
        saveSubLevel(LRU.front());

    for(unsigned int i=0;i<subs.size();++i)
    {
        ncSubVoxTree tmp(getFilename(subs[i].pos.xyz()));
        tmp.subDivide();
        tmp.save();
    }
}

void ncVoxTree::createColors() {
    while(LRU.size()>0)
        saveSubLevel(LRU.front());

    for(unsigned int i=0;i<subs.size();++i)
    {
        ncSubVoxTree tmp(getFilename(subs[i].pos.xyz()));
        tmp.createColors();
        tmp.save();
    }
}

void ncVoxTree::copy(std::string fname_out,std::string fname_in,unsigned int mask) {
    ncVoxTree vin(fname_in  , false , false);
    ncVoxTree vout(fname_out, false , true , mask , vin);

    //fprintf(stderr,"ncVoxTree::copy(%s,%s,%d) -> %d %d\n",fname_out.c_str(),fname_in.c_str(),(mask&NC_COLOR? 1 : 0),(vout.data_mask&NC_COLOR? 1 : 0),(vin.data_mask&NC_COLOR? 1 : 0));

    for (unsigned int i=0;i<vin.subs.size();++i)
    {
        ncSubVoxTree sub_in(vin.getFilename(vin.subs[i].pos.xyz()));
        ncSubVoxTree sub_out(vout.getFilename(vout.subs[i].pos.xyz()) , mask , sub_in);
        sub_out.save();
    }

    vout.save();
}

std::vector<ncc::v3i >  ncVoxTree::getSubLevelsAtCameraPosition(ncc::v3d campos) {

    //fprintf(stderr,"+setCameraPosition(%f %f %f)\n",campos.x,campos.y,campos.z);
    ncc::v3i cpos = 1000.0*(getTransform().inverse()*campos);
    int submask = ~((1<<sublevel)-1);
    cpos.x &= submask;
    cpos.y &= submask;
    cpos.z &= submask;

    std::vector<ncc::v3i> out;

    for (int z = -1 ; z<=1 ; ++z)
        for (int y = -1 ; y<=1 ; ++y)
            for (int x = -1 ; x<=1 ; ++x)
            {
                ncc::v3i tmp(cpos.x+x*(1<<sublevel) , cpos.y+y*(1<<sublevel) , cpos.z+z*(1<<sublevel) );
                if ((tmp.x>=0) && (tmp.x<(1<<maxlevel)) && (tmp.y>=0) && (tmp.y<(1<<maxlevel)) && (tmp.z>=0) && (tmp.z<(1<<maxlevel)))
                    out.push_back(tmp);
            }

    return out;

}

void ncVoxTree::setCameraPosition(ncc::v3d campos) {

    //fprintf(stderr,"+setCameraPosition(%f %f %f)\n",campos.x,campos.y,campos.z);
    std::vector<ncc::v3i> sublist = getSubLevelsAtCameraPosition(campos);

    for (unsigned int i=0;i<sublist.size();++i)
        addSubLevel(sublist[i]);

    //fprintf(stderr,"-setCameraPosition(%f %f %f) -> %d\n",campos.x,campos.y,campos.z,changed);
}

double ncVoxTree::getCameraDistance(ncc::v3d campos,ncc::v4i subl) {

    return ( 0.001*(ncc::v3d(pmin+subl.xyz())+0.5*ncc::v3d(1<<subl.w,1<<subl.w,1<<subl.w)) - campos).norm();
}

void ncVoxTree::setDataMask(unsigned int mask) {
    data_mask = mask;
}





