#include "ncassimp.h"

ncAssimpMesh::ncAssimpMesh(const aiMesh* paiMesh) {

    materialIndex = paiMesh->mMaterialIndex;

    unsigned int nrvertices = paiMesh->mNumVertices;
    vertices.clear();
    vertices.resize(nrvertices);

    //fprintf(stderr,"#vertices = %d\n",nrvertices);

    for (unsigned int i = 0 ; i < nrvertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        vertices[i] = ncc::v3f(pPos->x, pPos->y, pPos->z);
        //if (i==0)
        //    fprintf(stderr,"vertices[%d] = %f %f %f\n",i,pPos->x, pPos->y, pPos->z);
    }


	hasColors = paiMesh->HasVertexColors(0);
    if (hasColors)
    {
        colors.clear();
        colors.resize(nrvertices);
        for (unsigned int i = 0 ; i < nrvertices ; i++) {

        	const aiColor4D* mColor = &(paiMesh->mColors[0][i]);
        	colors[i] = ncc::v4f(mColor->r, mColor->g, mColor->b, mColor->a);
        }
    }

    hasTexCoords = paiMesh->HasTextureCoords(0);
    //fprintf(stderr,"hasTexCoords = %d\n",hasTexCoords);
    if (hasTexCoords)
    {
        texcoords.clear();
        texcoords.resize(nrvertices);
        for (unsigned int i = 0 ; i < nrvertices ; i++) {
            const aiVector3D* pTexCoord = &(paiMesh->mTextureCoords[0][i]);
            texcoords[i] = ncc::v2f(pTexCoord->x, pTexCoord->y);
        }
    }

    unsigned int nrfaces    = paiMesh->mNumFaces;
    //fprintf(stderr,"#faces = %d\n",nrfaces);
    indices.clear();
    indices.resize(nrfaces);

    for (unsigned int i = 0 ; i <nrfaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        indices[i] = ncc::v3i(Face.mIndices[0],Face.mIndices[1],Face.mIndices[2]);
    }

    glGenBuffers(1,&v_pbo);
    glBindBuffer(GL_ARRAY_BUFFER,v_pbo);
    glBufferData(GL_ARRAY_BUFFER,nrvertices*sizeof(ncc::v3f),&(vertices[0].x),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    if (hasColors)
    {
        glGenBuffers(1,&c_pbo);
        glBindBuffer(GL_ARRAY_BUFFER,c_pbo);
        glBufferData(GL_ARRAY_BUFFER,nrvertices*sizeof(ncc::v4f),&(colors[0].x),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

    if (hasTexCoords)
    {
        glGenBuffers(1,&t_pbo);
        glBindBuffer(GL_ARRAY_BUFFER,t_pbo);
        glBufferData(GL_ARRAY_BUFFER,nrvertices*sizeof(ncc::v2f),&(texcoords[0].x),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

    glGenBuffers(1,&i_pbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,i_pbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,nrfaces*sizeof(ncc::v3i),&(indices[0][0]),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, v_pbo );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0 );
	glEnableVertexAttribArray(0);

    if (hasTexCoords)
    {
        glBindBuffer(GL_ARRAY_BUFFER, t_pbo );
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glEnableVertexAttribArray(1);
    }

    if (hasColors)
    {
        glBindBuffer(GL_ARRAY_BUFFER, c_pbo );
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

ncAssimpMesh::~ncAssimpMesh() {
    if (vao)
        glDeleteVertexArrays(1,&vao);
    if (v_pbo)
        glDeleteBuffers(1,&v_pbo);
    if (c_pbo)
        glDeleteBuffers(1,&c_pbo);
    if (t_pbo)
        glDeleteBuffers(1,&t_pbo);
    if (i_pbo)
        glDeleteBuffers(1,&i_pbo);

    vertices.clear();
    texcoords.clear();
    indices.clear();
}

int                  ncAssimp::refcount = 0;
ncPtr<ncQtProgram>   ncAssimp::assimp_prog;

ncAssimp::ncAssimp() {
    init();
}

ncAssimp::ncAssimp(std::string fname) {
    init();
    ncModel::load(fname);
}

ncAssimp::~ncAssimp() {
    if (--refcount == 0)
    {
        assimp_prog.reset();
    }
}

void ncAssimp::init() {

    type = NC_ASSIMP;

    pScene  = 0;

    if (refcount++ == 0)
    {
        Q_INIT_RESOURCE(ncmodel_shaders);
        assimp_prog = new ncQtProgram(":/shaders/assimp.vert",":/shaders/assimp.geom",":/shaders/assimp.frag");
    }
}

void ncAssimp::clear() {
    for (unsigned int i=0;i<meshes.size();++i)
        delete meshes[i];
    meshes.clear();
    materials.clear();
    if (pScene)
    {
        Importer.FreeScene();
        pScene = 0;
    }
    ncModel::clear();

}

bool ncAssimp::v_load(std::string fname) {

        char transform_filename[1024];
        sprintf(transform_filename  ,"%s.tr",fname.c_str());

        if (ncc::checkFile(transform_filename))
        {
            fprintf(stderr,"Found transform file %s...\n",transform_filename);
            std::ifstream ofile1(transform_filename,std::ios_base::in);
            ofile1.setf(std::ios::fixed,std::ios::floatfield);
            for (int r=0;r<4;++r)
                for (int c=0;c<4;++c)
                    ofile1 >> transform[c][r] >> std::ws;
            ofile1.close();
        }

        pScene = Importer.ReadFile(fname.c_str() , aiProcess_Triangulate);

        if (pScene)
        {
            unsigned int nrmeshes    = pScene->mNumMeshes;
            unsigned int nrmaterials = pScene->mNumMaterials;

            /*
            fprintf(stderr,"Assimp Importer Info:\n");
            fprintf(stderr,"---------------------\n");
            fprintf(stderr,"#meshes     : %u\n",nrmeshes);
            fprintf(stderr,"#materials  : %u\n",nrmaterials);
            */

            //! initialize meshes

			meshes.resize(nrmeshes);
            for (unsigned int i = 0;i<nrmeshes;++i)
            {
                const aiMesh* paiMesh = pScene->mMeshes[i];
                meshes[i] = new ncAssimpMesh(paiMesh);

                //fprintf(stderr,"mesh %d -> material %d\n",i,meshes[i]->materialIndex);
            }

			//!initialize materials

            materials.resize(nrmaterials);
            for (unsigned int i = 0 ; i < nrmaterials ; i++) {
                const aiMaterial* pMaterial = pScene->mMaterials[i];

                if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                    aiString Path;

                    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                        std::string FullPath = ncc::getDirName(fname) + Path.data;

                        //fprintf(stderr,"\rReading material %s",FullPath.c_str());
                        //fprintf(stderr,"Reading material %d -> %s\n",i,FullPath.c_str());

                        ncImageBuffer<ncc::v3ub> flipped;
                        ncc::readImage(FullPath.c_str(),flipped);
                        ncImageBuffer<ncc::v3ub> tmp(flipped.width,flipped.height);
                        for (unsigned int y=0;y<flipped.height;++y)
                            memcpy(&(tmp[y*flipped.width][0]),&(flipped[(flipped.height-1-y)*flipped.width][0]),flipped.width*sizeof(ncc::v3ub));

                        materials[i].tex = new ncTexImage2D(GL_RGB8,tmp.width,tmp.height);
                        materials[i].tex->setData(&tmp[0][0]);
                        materials[i].tex->setFilters(GL_LINEAR,GL_LINEAR);
                    }
				}
            }

            //fprintf(stderr,"\n");
            valid = true;
        }
        else
            printf("Error parsing '%s': '%s'\n", fname.c_str(), Importer.GetErrorString());

    return true;
}

void ncAssimp::v_draw(ncc::m4x4d KGL , ncc::RTd MVi) {

    ncc::RTd  MVi_ = MVi*transform;
	assimp_prog->Activate();
	assimp_prog->setUniform("KGL",KGL);

    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    recursive_render(pScene, pScene->mRootNode , KGL , MVi_);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    assimp_prog->Deactivate();
}

void ncAssimp::recursive_render (const struct aiScene *sc, const struct aiNode* nd , ncc::m4x4d KGL , ncc::RTd MVi) {

	aiMatrix4x4 m1 = nd->mTransformation;
    ncc::m4x4f m2 = *((ncc::m4x4f*)&m1);
    ncc::m4x4d m3 = m2.transpose();
    ncc::RTd   m4 = *((ncc::RTd*)&m3);
    MVi = MVi*m4;

    assimp_prog->setUniform("MVi", MVi);

	for (unsigned int m = 0; m < nd->mNumMeshes; ++m) {
		int i = nd->mMeshes[m];
		ncTexImage* tex = materials[meshes[i]->materialIndex].tex;

	    if (tex)
	    {
	    	assimp_prog->setSampler("tex", tex , 0);
	    	assimp_prog->setUniform("textured",true);
	    }
	    else
	    	assimp_prog->setUniform("textured",false);

	    glBindVertexArray(meshes[i]->vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->i_pbo);
        glDrawElements(GL_TRIANGLES, 3*meshes[i]->indices.size(), GL_UNSIGNED_INT, 0 );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
        glBindVertexArray(0);

    }

	for (unsigned int n = 0; n < nd->mNumChildren; ++n)
		recursive_render(sc, nd->mChildren[n] , KGL , MVi);

}

void ncAssimp::collect( std::vector<ncc::v3f>& vert , std::vector<ncc::v3i>& ind) {
    vert.clear();
    ind.clear();
	ncc::RTd  MVi = ncc::RTd::getIdentity();
	recursive_collect(vert , ind , pScene , pScene->mRootNode , MVi);
}

void ncAssimp::recursive_collect( std::vector<ncc::v3f>& vert , std::vector<ncc::v3i>& ind , const struct aiScene *sc, const struct aiNode* nd , ncc::RTd MVi) {

    //fprintf(stderr,"recursive_collect #children = %d\n",nd->mNumChildren);

    aiMatrix4x4 m1 = nd->mTransformation;
    ncc::m4x4f m2 = *((ncc::m4x4f*)&m1);
    ncc::m4x4d m3 = m2.transpose();
    ncc::RTd   m4 = *((ncc::RTd*)&m3);

    MVi = MVi*m4;

    for (unsigned int m = 0; m < nd->mNumMeshes; ++m) {

        int offset = vert.size();

		int mi = nd->mMeshes[m];

        unsigned int nrvert = meshes[mi]->vertices.size();
        unsigned int nrind  = meshes[mi]->indices.size();

	    for (unsigned int v=0;v<nrvert;++v)
	    {
	        ncc::v3f vt = meshes[mi]->vertices[v];
            vert.push_back(MVi*vt);
	    }

	    for (unsigned int i=0;i<nrind;++i)
        {
	        ncc::v3i it = meshes[mi]->indices[i];
            ind.push_back(ncc::v3i(it[0]+offset,it[1]+offset,it[2]+offset));
        }
    }

	for (unsigned int n = 0; n < nd->mNumChildren; ++n)
		recursive_collect(vert,ind,sc, nd->mChildren[n] , MVi);
}
