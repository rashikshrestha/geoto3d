#include "ncprogram.h"

ncProgram::ncProgram() {
    vshader = 0;
    gshader = 0;
    fshader = 0;
	program = glCreateProgram();
}

ncProgram::ncProgram(const char* vert, const char* geom, const char* frag, bool source) {
    vshader = 0;
    gshader = 0;
    fshader = 0;

    program = glCreateProgram();
    if (source)
        loadSources(vert, geom, frag);
    else
        loadFiles(vert, geom, frag);
}

ncProgram::~ncProgram() {

    if(vshader)
        glDeleteShader(vshader);
    if(gshader)
        glDeleteShader(gshader);
    if(fshader)
        glDeleteShader(fshader);

    glDeleteProgram(program);
}

GLuint ncProgram::getID() {
    return program;
}

void ncProgram::loadFiles(const char* vfile, const char* gfile, const char* ffile) {

    //fprintf(stderr,"ncProgram::loadFiles( %s , %s , %s )\n",vfile,gfile,ffile);

    if (vfile) {
        vshader = glCreateShader(GL_VERTEX_SHADER);

        std::vector<std::string> vfiles   = ncc::split(std::string(vfile), ':');
        std::vector<std::string> vsources = std::vector<std::string>(vfiles.size());
        const char ** buf = new const char*[vfiles.size()];
        for (unsigned int i=0;i<vfiles.size();++i)
        {
            readFile(vfiles[i].c_str(),vsources[i]);
            buf[i] = vsources[i].c_str();
        }
        compile(buf,vfiles.size(),vshader);
        delete[] buf;
    }

    if (gfile) {
        gshader = glCreateShader(GL_GEOMETRY_SHADER);

        std::vector<std::string> gfiles   = ncc::split(std::string(gfile), ':');
        std::vector<std::string> gsources = std::vector<std::string>(gfiles.size());
        const char ** buf = new const char*[gfiles.size()];
        for (unsigned int i=0;i<gfiles.size();++i)
        {
            readFile(gfiles[i].c_str(),gsources[i]);
            buf[i] = gsources[i].c_str();
        }
        compile(buf,gfiles.size(),gshader);
        delete[] buf;
    }

    if (ffile) {
        fshader = glCreateShader(GL_FRAGMENT_SHADER);

        std::vector<std::string> ffiles   = ncc::split(std::string(ffile), ':');
        std::vector<std::string> fsources = std::vector<std::string>(ffiles.size());
        const char ** buf = new const char*[ffiles.size()];
        for (unsigned int i=0;i<ffiles.size();++i)
        {
            readFile(ffiles[i].c_str(),fsources[i]);
            buf[i] = fsources[i].c_str();
        }
        compile(buf,ffiles.size(),fshader);
        delete[] buf;
    }
    link();
}

void ncProgram::loadFiles(std::vector<std::string> vfiles, std::vector<std::string> gfiles, std::vector<std::string> ffiles) {

    std::string vs,gs,fs;
    for (unsigned int i = 0;i<vfiles.size();++i)
        vs+= vfiles[i]+" ";
    for (unsigned int i = 0;i<gfiles.size();++i)
        gs+= gfiles[i]+" ";
    for (unsigned int i = 0;i<ffiles.size();++i)
        fs+= ffiles[i]+" ";

    //fprintf(stderr,"ncProgram::loadFiles( %s , %s , %s )\n",vs.c_str() , gs.c_str() , fs.c_str());

    if (vfiles.size()>0) {
        vshader = glCreateShader(GL_VERTEX_SHADER);

        std::vector<std::string> vsources = std::vector<std::string>(vfiles.size());
        const char ** buf = new const char*[vfiles.size()];
        for (unsigned int i=0;i<vfiles.size();++i)
        {
            readFile(vfiles[i].c_str(),vsources[i]);
            buf[i] = vsources[i].c_str();
        }
        compile(buf,vfiles.size(),vshader);
        delete[] buf;
    }

    if (gfiles.size()>0) {
        gshader = glCreateShader(GL_GEOMETRY_SHADER);

        std::vector<std::string> gsources = std::vector<std::string>(gfiles.size());
        const char ** buf = new const char*[gfiles.size()];
        for (unsigned int i=0;i<gfiles.size();++i)
        {
            readFile(gfiles[i].c_str(),gsources[i]);
            buf[i] = gsources[i].c_str();
        }
        compile(buf,gfiles.size(),gshader);
        delete[] buf;
    }

    if (ffiles.size()>0) {
        fshader = glCreateShader(GL_FRAGMENT_SHADER);

        std::vector<std::string> fsources = std::vector<std::string>(ffiles.size());
        const char ** buf = new const char*[ffiles.size()];
        for (unsigned int i=0;i<ffiles.size();++i)
        {
            readFile(ffiles[i].c_str(),fsources[i]);
            buf[i] = fsources[i].c_str();
        }
        compile(buf,ffiles.size(),fshader);
        delete[] buf;
    }
    link();
}

void ncProgram::loadSources(const char* vsrc, const char* gsrc, const char* fsrc) {

    if (vsrc) {
        vshader = glCreateShader(GL_VERTEX_SHADER);
        compile(&vsrc,1,vshader);
    }

    if (gsrc) {
        gshader = glCreateShader(GL_GEOMETRY_SHADER);
        compile(&gsrc,1,gshader);
    }

    if (fsrc) {
        fshader = glCreateShader(GL_FRAGMENT_SHADER);
        compile(&fsrc,1,fshader);
    }
    link();
}

void ncProgram::loadSources(std::vector<std::string> vsrc, std::vector<std::string> gsrc, std::vector<std::string> fsrc) {

    if (vsrc.size()>0) {
        vshader = glCreateShader(GL_VERTEX_SHADER);
        const char ** buf = new const char*[vsrc.size()];
        for (unsigned int i=0;i<vsrc.size();++i)
            buf[i] = vsrc[i].c_str();
        compile(buf,vsrc.size(),vshader);
        delete[] buf;
    }

    if (gsrc.size()>0) {
        gshader = glCreateShader(GL_GEOMETRY_SHADER);
        const char ** buf = new const char*[gsrc.size()];
        for (unsigned int i=0;i<gsrc.size();++i)
            buf[i] = gsrc[i].c_str();
        compile(buf,gsrc.size(),gshader);
        delete[] buf;
    }

    if (fsrc.size()>0) {
        fshader = glCreateShader(GL_FRAGMENT_SHADER);
        const char ** buf = new const char*[fsrc.size()];
        for (unsigned int i=0;i<fsrc.size();++i)
            buf[i] = fsrc[i].c_str();
        compile(buf,fsrc.size(),fshader);
        delete[] buf;
    }

    link();
}

void ncProgram::compile(const char ** buf, int count , GLuint shader) {

    //fprintf(stderr,"Start Compiling...\n");
    //for (int i=0;i<count;++i)
    //    fprintf(stderr,"%d : \n%s\n\n",i,buf[i]);


    glShaderSource(shader, count, buf, 0);
    glCompileShader(shader);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
    if (status == GL_FALSE) {
        fprintf(stderr,"f status %d\n",status);
        glGetShaderInfoLog(shader, 1000, &infolength, infolog);
        fprintf(stderr,"infolog:\n%s\n",infolog);
    }
    glAttachShader(program,shader);

    //fprintf(stderr,"End Compiling...\n");

}

void ncProgram::Activate() {
    glUseProgram(program);
}

void ncProgram::Deactivate() {
    glUseProgram(0);
}

void ncProgram::link() {
    glLinkProgram(program);
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if (status == GL_FALSE) {
        glGetProgramInfoLog(program, 1000, &infolength, infolog);
        fprintf(stderr,"infolog:\n%s\n",infolog);
    }
}

void ncProgram::readFile(const char* file, std::string& buf) {

    std::string tmp;
	std::string line;
	std::ifstream in(file);

	if (in.is_open())
	{
		while(std::getline(in,line))
		{
			tmp += line;
			tmp += "\n";
		}
		buf = tmp;
	}
	else
		fprintf(stderr,"File %s does not exist\n",file);
}

void ncProgram::setUniform(const char* var, const bool val) {
    glUniform1i(glGetUniformLocation(program, var), val);
}

void ncProgram::setUniform(const char* var, const bool* val, unsigned int count) {
    glUniform1iv(glGetUniformLocation(program, var), count, (const int*)val);
}

void ncProgram::setUniform(const char* var, const int val) {
    glUniform1i(glGetUniformLocation(program, var), val);
}

void ncProgram::setUniform(const char* var, const int* val, unsigned int count) {
    glUniform1iv(glGetUniformLocation(program, var), count, val);
}

void ncProgram::setUniform(const char* var, const unsigned int val) {
    glUniform1ui(glGetUniformLocation(program, var), val);
}

void ncProgram::setUniform(const char* var, const unsigned int* val, unsigned int count) {
    glUniform1uiv(glGetUniformLocation(program, var), count, val);
}

void ncProgram::setUniform(const char* var, const float val) {
    glUniform1f(glGetUniformLocation(program, var), val);
}

void ncProgram::setUniform(const char* var, const float* val, unsigned int count) {
    glUniform1fv(glGetUniformLocation(program, var), count, val);
}

void ncProgram::setUniform(const char* var, const double val) {
    glUniform1f(glGetUniformLocation(program, var), (float)val);
}

void ncProgram::setUniform(const char* var, const ncc::v2f & vec) {
	glUniform2fv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v2f * vec, unsigned int count) {
	glUniform2fv(glGetUniformLocation(program, var), count, (float*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v2d & vec) {
    ncc::v2f tmp = vec;
	setUniform(var, tmp);
}

void ncProgram::setUniform(const char* var, const ncc::v2i & vec) {
	glUniform2iv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v2i * vec, unsigned int count) {
	glUniform2iv(glGetUniformLocation(program, var), count, (int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v2ui & vec) {
	glUniform2uiv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v2ui * vec, unsigned int count) {
	glUniform2uiv(glGetUniformLocation(program, var), count, (unsigned int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v3f & vec) {
	glUniform3fv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v3f * vec, unsigned int count) {
	glUniform3fv(glGetUniformLocation(program, var), count, (float*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v3d & vec) {
    ncc::v3f tmp = vec;
	setUniform(var, tmp);
}

void ncProgram::setUniform(const char* var, const ncc::v3i & vec) {
	glUniform3iv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v3i * vec, unsigned int count) {
	glUniform3iv(glGetUniformLocation(program, var), count, (int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v3ui & vec) {
	glUniform3uiv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v3ui * vec, unsigned int count) {
	glUniform3uiv(glGetUniformLocation(program, var), count, (unsigned int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v4f & vec) {
    glUniform4fv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v4f * vec, unsigned int count) {
    glUniform4fv(glGetUniformLocation(program, var), count, (float*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v4d & vec) {
    ncc::v4f tmp = vec;
	setUniform(var, tmp);
}

void ncProgram::setUniform(const char* var, const ncc::v4i & vec) {
    glUniform4iv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v4i * vec, unsigned int count) {
    glUniform4iv(glGetUniformLocation(program, var), count, (int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::v4ui & vec) {
    glUniform4uiv(glGetUniformLocation(program, var), 1, &vec[0]);
}

void ncProgram::setUniform(const char* var, const ncc::v4ui * vec, unsigned int count) {
    glUniform4uiv(glGetUniformLocation(program, var), count, (unsigned int*)vec);
}

void ncProgram::setUniform(const char* var, const ncc::m4x4f& mat) {
    glUniformMatrix4fv(glGetUniformLocation(program, var), 1, false, &mat[0][0]);
}

void ncProgram::setUniform(const char* var, const ncc::m4x4f* mat, unsigned int count) {
    glUniformMatrix4fv(glGetUniformLocation(program, var), count, false, (float*)mat);
}

void ncProgram::setUniform(const char* var, ncPBO* pbo, GLenum mode) {

    pbo->makeResident(mode);
    glUniformui64NV(glGetUniformLocation(program, var) , pbo->getGpuAddress());
}

void ncProgram::setSampler(const char* var, const ncTexImage* tex, int index) {
    glUniform1i(glGetUniformLocation(program, var), index);
    tex->bind(index);
}

void ncProgram::setFragDataLocation(const char *name, unsigned int colorNumber) {
    glBindFragDataLocation( program, colorNumber, name );
}
