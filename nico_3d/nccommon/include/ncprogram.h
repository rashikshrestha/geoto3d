#ifndef __NC_PROGRAM__
#define __NC_PROGRAM__

#include <vector>
#include "ncgl.h"
#include "ncmath.h"
#include "ncteximage.h"
#include "ncpbo.h"

class ncProgram {

        public:
    ncProgram();
    ncProgram(const char* vert, const char* geom, const char* frag, bool source = 0);
    ~ncProgram();

	void loadFiles(const char* vfile, const char* gfile, const char* ffile);
	void loadFiles(std::vector<std::string> vfiles, std::vector<std::string> gfiles, std::vector<std::string> ffiles);
	void loadSources(const char* vsrc, const char* gsrc, const char* fsrc);
    void loadSources(std::vector<std::string> vsrc, std::vector<std::string> gsrc, std::vector<std::string> fsrc);
	GLuint getID();


    void Activate();
	static void Deactivate();

    void setUniform(const char* var, const bool             val);
    void setUniform(const char* var, const bool*            val, unsigned int count = 1);
    void setUniform(const char* var, const int              val);
    void setUniform(const char* var, const int*             val, unsigned int count = 1);
    void setUniform(const char* var, const unsigned int     val);
    void setUniform(const char* var, const unsigned int*    val, unsigned int count = 1);
    void setUniform(const char* var, const float            val);
    void setUniform(const char* var, const float*           val, unsigned int count = 1);
    void setUniform(const char* var, const double           val);
	void setUniform(const char* var, const ncc::v2f & vec);
	void setUniform(const char* var, const ncc::v2f * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v2d & vec);
	void setUniform(const char* var, const ncc::v2d * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v2i & vec);
	void setUniform(const char* var, const ncc::v2i * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v2ui & vec);
	void setUniform(const char* var, const ncc::v2ui * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v3f & vec);
	void setUniform(const char* var, const ncc::v3f * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v3d & vec);
	void setUniform(const char* var, const ncc::v3d * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v3i & vec);
	void setUniform(const char* var, const ncc::v3i * vec, unsigned int count = 1);
	void setUniform(const char* var, const ncc::v3ui & vec);
	void setUniform(const char* var, const ncc::v3ui * vec, unsigned int count = 1);
    void setUniform(const char* var, const ncc::v4f & vec);
    void setUniform(const char* var, const ncc::v4f * vec, unsigned int count = 1);
    void setUniform(const char* var, const ncc::v4d & vec);
    void setUniform(const char* var, const ncc::v4d * vec, unsigned int count = 1);
    void setUniform(const char* var, const ncc::v4i & vec);
    void setUniform(const char* var, const ncc::v4i * vec, unsigned int count = 1);
    void setUniform(const char* var, const ncc::v4ui & vec);
    void setUniform(const char* var, const ncc::v4ui * vec, unsigned int count = 1);
    void setUniform(const char* var, const ncc::m4x4f& mat);
    void setUniform(const char* var, const ncc::m4x4f* mat, unsigned int count = 1);

    void setUniform(const char* var, ncPBO* pbo, GLenum mode = GL_READ_WRITE);

    void setSampler(const char* var, const ncTexImage* tex, int index);
    void setFragDataLocation(const char *name , unsigned int colorNumber);

        private:
    void readFile(const char* file, std::string& buf);
    void compile(const char ** buf, int count , GLuint shader);

        private:

    void link();

    std::string vshader_src;
    std::string gshader_src;
    std::string fshader_src;

    GLuint vshader;
    GLuint gshader;
    GLuint fshader;

    GLuint program;

    int 	status;
    char 	infolog[10000];
    int 	infolength;
};

#endif
