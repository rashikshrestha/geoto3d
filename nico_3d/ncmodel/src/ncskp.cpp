#include "ncskp.h"

int                  ncSKP::refcount = 0;
ncPtr<ncQtProgram>   ncSKP::skp_prog;

ncSKPd::ncSKPd() {
}

ncSKPd::ncSKPd(SUEntitiesRef & entities , ncc::RTd transform_) {
	loadEntities(entities,transform_);
}

ncSKPd::~ncSKPd() {
    clear();
}

void ncSKPd::clear() {
	transform.setIdentity();

    mesh.reset();
    d_vert.reset();
    d_ind.reset();
	h_vert.clear();
	h_ind.clear();

	for (unsigned int i=0;i<groups.size();++i)
		delete groups[i];
	groups.clear();

	faces.clear();
}

void ncSKPd::loadEntities(SUEntitiesRef & entities , ncc::RTd transform_) {

	transform = transform_;

	unsigned int nr_faces;
	unsigned int nr_tri;
	unsigned int nr_vert;
	unsigned int nr_ind;
	unsigned int nr_groups;

	SUEntitiesGetNumFaces(entities, &nr_faces);

	if (nr_faces > 0) {

		std::vector<SUFaceRef> su_faces(nr_faces);
		SUEntitiesGetFaces(entities, nr_faces, &su_faces[0], &nr_faces);

		//memcpy(su_faces[0].ptr,su_faces[1].ptr,136);

		//for (unsigned int jj=1;jj<nr_faces;++jj)
		//	fprintf(stderr,"su_faces[%d] = %16X\n",jj,int(su_faces[jj].ptr)-int(su_faces[jj-1].ptr));

		for (size_t i = 0; i < nr_faces; i++) {
		      

			/*
			SUTextureWriterRef tw;
			SUTextureWriterCreate(&tw);

			SUUVHelperRef uv_helper = SU_INVALID;
			SUFaceGetUVHelper(su_faces[i],true,false,tw,&uv_helper); 
			SUUVHelperRelease(&uv_helper);

			SUTextureWriterRelease(&tw);
			*/

			/*
			SUMaterialRef material = SU_INVALID;
			SUMaterialCreate(&material);
			SUTextureRef texture = SU_INVALID;
			SUTextureCreateFromFile(&texture, "C:\\Users\\nico\\Desktop\\GeoToTexture\\data\\texture.jpg", 1.0, 1.0);
			SUMaterialSetTexture(material, texture);
			SUFaceSetFrontMaterial(face, material);

			SUGeometryInputRef geom_input = SU_INVALID;
			SUGeometryInputCreate(&geom_input);
			SUGeometryInputRelease(&geom_input);
			*/


			SUMeshHelperRef su_mesh	= SU_INVALID;
			SUMeshHelperCreate(&su_mesh, su_faces[i]);

			SUMeshHelperGetNumTriangles(su_mesh, &nr_tri);
			SUMeshHelperGetNumVertices(su_mesh, &nr_vert);
			nr_ind = 3*nr_tri;

			std::vector<unsigned int>	ind(nr_ind);
			std::vector<SUPoint3D>		vert(nr_vert);

			SUMeshHelperGetVertexIndices(su_mesh, nr_ind, &ind[0], &nr_ind);
			SUMeshHelperGetVertices(su_mesh, nr_vert, &vert[0], &nr_vert);
			SUMeshHelperRelease(&su_mesh);

			int i_offset = h_ind.size();
			faces.push_back(ncSKPface(i_offset,nr_tri));

			int v_offset = h_vert.size();
			for (unsigned int v=0;v<nr_vert;++v)
				h_vert.push_back(ncc::v3f(vert[v].x,vert[v].y,vert[v].z));

			for (unsigned int t=0;t<nr_tri;++t)
				h_ind.push_back(ncc::v3ui(ind[3*t+0]+v_offset,ind[3*t+1]+v_offset,ind[3*t+2]+v_offset));

		}
	}

	if (h_vert.size()>0)
	{
		pmin = h_vert[0];
		pmax = h_vert[0];
		for (unsigned int v=0;v<h_vert.size();++v)
		{
			pmin = (std::min)(pmin,h_vert[v]);
			pmax = (std::max)(pmax,h_vert[v]);
		}

		d_vert	= new ncPBO;
		d_vert->setup(h_vert.size(),&h_vert[0]);

		mesh	= new ncMesh;
		mesh->bindAttribF(0,d_vert,3,GL_FLOAT,GL_FALSE, sizeof(ncc::v3f),0);
	}
			
	if (h_ind.size()>0)
	{
		d_ind	= new ncIndexArray;
		d_ind->setup(h_ind.size(),&h_ind[0]);
	}

	SUEntitiesGetNumGroups(entities, &nr_groups);

	if (nr_groups>0) {

		fprintf(stderr,"Found %d groups\n",nr_groups);

		std::vector<SUGroupRef> su_groups(nr_groups);
		SUEntitiesGetGroups(entities, nr_groups,&su_groups[0],&nr_groups);

		for (unsigned int i=0;i<nr_groups;++i)
		{
			/*
			char	my_str[1024];
			size_t	my_str_len;

			SUStringRef su_string = SU_INVALID;
			SUStringCreate(&su_string);
			SUGroupGetName(su_groups[i], &su_string);
			SUStringGetUTF8(su_string, 1024, my_str, &my_str_len);
			SUStringRelease(&su_string);

			fprintf(stderr,"group name = %s\n",my_str);
			*/

			SUEntitiesRef g_entities = SU_INVALID;
			SUGroupGetEntities(su_groups[i], &g_entities);

			SUTransformation su_transform = SU_INVALID;
			SUGroupGetTransform(su_groups[i],&su_transform);

			ncc::RTd g_transform = convert(su_transform);
			groups.push_back(new ncSKPd(g_entities,g_transform));
		}
	}
}

ncc::RTd ncSKPd::convert(SUTransformation su_transform) {

	ncc::RTd g_transform = (*((ncc::RTd*)&su_transform));

	for (unsigned int i=0;i<4;++i)
		for (unsigned int j=0;j<4;++j)
			g_transform[i][j]/=g_transform[3][3];

	return g_transform;
}

void ncSKPd::draw(ncc::m4x4d KGL , ncc::RTd MVi) {

	MVi = MVi*transform;

	if (mesh)
    {
        ncc::m4x4d MVP = KGL*MVi;
		ncSKP::skp_prog->Activate();
        ncSKP::skp_prog->setUniform("MVP",MVP);
        //mesh->draw(GL_TRIANGLES,d_ind);
        
		for (unsigned int i=0;i<faces.size();++i)
	        mesh->draw(GL_TRIANGLES,d_ind,3*faces[i].tri_count,3*faces[i].tri_start);
		
		ncSKP::skp_prog->Deactivate();
    }
	for (unsigned int i=0;i<groups.size();++i)
		groups[i]->draw(KGL,MVi);
}




ncSKP::ncSKP(std::string fname) {
    if (refcount++ == 0)
    {
		SUInitialize();
        Q_INIT_RESOURCE(ncmodel_shaders);
        skp_prog = new ncQtProgram(":/shaders/skp.vert",":/shaders/skp.geom",":/shaders/skp.frag");
    }
    load(fname);
}

ncSKP::~ncSKP() {
    clear();
    if (--refcount == 0)
    {
        skp_prog.reset();
		SUTerminate();
    }
}

void ncSKP::clear() {
    filename.clear();
	ncSKPd::clear();
}

void ncSKP::load(std::string fname) {

		clear();

    if (!fname.empty() && ncc::checkFile(fname.c_str()))
    {
        filename = fname;

		// Load the model from a file
		SUModelRef model	= SU_INVALID;
		SUResult res		= SUModelCreateFromFile(&model, fname.c_str());

		// It's best to always check the return code from each SU function call.
		// Only showing this check once to keep this example short.
		//if (res != SU_ERROR_NONE)
		//	return 1;

		//SUModelUnits unit;
		//SUModelGetUnits(model, &unit);
		//fprintf(stderr,"SKP unit %08X %08X %08X\n",unit,SUModelUnits_Meters,SUModelUnits_Feet);

		unsigned int nr_layer;
		SUModelGetNumLayers(model, &nr_layer);
		fprintf(stderr,"Found %d layers\n",nr_layer);

		unsigned int nr_compdef;
		SUModelGetNumComponentDefinitions(model,&nr_compdef);
		fprintf(stderr,"Found %d component definitions\n",nr_compdef);

		// Get the entity container of the model.
		SUEntitiesRef entities = SU_INVALID;
		SUModelGetEntities(model, &entities);
	
		loadEntities(entities,ncc::RTd::getIdentity());

		// Must release the model or there will be memory leaks
		SUModelRelease(&model);
	  		
	}
}

void ncSKP::save(std::string fname) {
	fprintf(stderr,"SKP Save not implemented\n");
}
