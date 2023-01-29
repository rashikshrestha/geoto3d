#include <liblas/capi/liblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ncmath.h"
#include "nccommon.h"
#include <vector>
#include <fstream>

#define LAS_FORMAT_10 0
#define LAS_FORMAT_11 1
#define LAS_FORMAT_12 2

bool        compressed      =   false;

static void loadMDL(std::string fname , ncc::RTd& ptransform , std::vector<ncc::v3f4ub>& pdata)
{

    unsigned int file_type ,file_version;

	std::ifstream file(fname.c_str(),std::ifstream::binary);
    //file.read((char*)&file_type        , sizeof(unsigned int           ));
    //file.read((char*)&file_version     , sizeof(unsigned int           ));
    file.read((char*)&ptransform[0][0] , sizeof(ncc::RTd               ));
    unsigned int nrpoints;
    file.read((char*)&nrpoints         , sizeof(unsigned int           ));
    pdata.resize(nrpoints);
    file.read((char*)&pdata[0].x       , nrpoints*sizeof(ncc::v3f4ub   ));
    file.close();
}

static void usage()
{

    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"    txt2las (version %s) usage:\n", LAS_GetVersion());
    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Parse a text file with a given format:\n");
    fprintf(stderr,"  txt2las -parse tsxyz lidar.txt\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Set the scale:\n");
    fprintf(stderr,"  txt2las --parse xyz --scale 0.02 -i lidar.txt -o lidar.laz\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"Set the xyz scale:\n");
    fprintf(stderr,"  txt2las --parse xsysz --verbose --xyz_scale 0.02 0.02 0.01 lidar.txt\n");
    fprintf(stderr,"\n");

    fprintf(stderr,"----------------------------------------------------------\n");
    fprintf(stderr," The '--parse txyz' flag specifies how to format each\n");
    fprintf(stderr," each line of the ASCII file. For example, 'txyzia'\n");
    fprintf(stderr," means that the first number of each line should be the\n");
    fprintf(stderr," gpstime, the next three numbers should be the x, y, and\n");
    fprintf(stderr," z coordinate, the next number should be the intensity\n");
    fprintf(stderr," and the next number should be the scan angle.\n");
    fprintf(stderr," The supported entries are:\n");
    fprintf(stderr,"   a - scan angle\n");
    fprintf(stderr,"   i - intensity\n");
    fprintf(stderr,"   n - number of returns for given pulse\n");
    fprintf(stderr,"   r - number of this return\n");
    fprintf(stderr,"   c - classification\n");
    fprintf(stderr,"   u - user data (does not currently work)\n");
    fprintf(stderr,"   p - point source ID\n");
    fprintf(stderr,"   e - edge of flight line\n");
    fprintf(stderr,"   d - direction of scan flag\n");
    fprintf(stderr,"   R - red channel of RGB color\n");
    fprintf(stderr,"   G - green channel of RGB color\n");
    fprintf(stderr,"   B - blue channel of RGB color\n\n");

    fprintf(stderr,"\n----------------------------------------------------------\n");
    fprintf(stderr," The '-scale 0.02' flag specifies the quantization. The\n");
    fprintf(stderr," default value of 0.01 means that the smallest increment\n");
    fprintf(stderr," two between coordinates is 0.01. If measurements are in\n");
    fprintf(stderr," meters this corresponds to centimeter accuracy, which is\n");
    fprintf(stderr," commonly considered sufficient for LIDAR data.\n");

    fprintf(stderr,"\n----------------------------------------------------------\n");
    fprintf(stderr," Other parameters such as '--xyz_offset 500000 2000000 0'\n");
    fprintf(stderr," or '-xyz_scale 0.02 0.02 0.01' or '-file_creation 67 2003'\n");
    fprintf(stderr," or '-system_identifier \"Airborne One Leica 50,000 Hz\"'\n");
    fprintf(stderr," or '-generating_software \"TerraScan\"' are available too.\n");

    fprintf(stderr, "For more information, see the full documentation for txt2las at:\n"
                    " http://liblas.org/browser/trunk/doc/txt2las.txt\n");
    fprintf(stderr,"----------------------------------------------------------\n");

}

static void VecUpdateMinMax3dv(double min[3], double max[3], const double v[3])
{
  if (v[0]<min[0]) min[0]=v[0]; else if (v[0]>max[0]) max[0]=v[0];
  if (v[1]<min[1]) min[1]=v[1]; else if (v[1]>max[1]) max[1]=v[1];
  if (v[2]<min[2]) min[2]=v[2]; else if (v[2]>max[2]) max[2]=v[2];
}

static void VecCopy3dv(double v[3], const double a[3])
{
  v[0] = a[0];
  v[1] = a[1];
  v[2] = a[2];
}

static int parse(const char* parse_string, const char* line, double* xyz, LASPointH point, double* gps_time)
{
    int temp_i;
    float temp_f;
    const char* p = parse_string;
    const char* l = line;

    LASColorH color = LASColor_Create();

    while (p[0])
    {
        /* // we expect the x coordinate */
        if (p[0] == 'x')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[0])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the y coordinate */
        else if (p[0] == 'y')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[1])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the x coordinate */
        else if (p[0] == 'z')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", &(xyz[2])) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect a string or a number that we don't care about */
        else if (p[0] == 's')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the intensity */
        else if (p[0] == 'i')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
            if (temp_f < 0.0f || temp_f > 65535.0f)
                fprintf(stderr,
                        "WARNING: intensity %g is out of range of unsigned short\n",
                        temp_f);
            LASPoint_SetIntensity(point, (int)temp_f);
/*          point->intensity = (unsigned short)temp_f; */
            /* // then advance to next white space*/
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the scan angle */
        else if (p[0] == 'a')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%f", &temp_f) != 1) return FALSE;
            if (temp_f < -128.0f || temp_f > 127.0f)
                fprintf(stderr,
                        "WARNING: scan angle %g is out of range of char\n",
                        temp_f);
            LASPoint_SetScanAngleRank(point, (int)temp_f);
/*          point->scan_angle_rank = (char)temp_f; */
            /* // then advance to next white space*/
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the number of returns of given pulse */
        else if (p[0] == 'n')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 7)
                fprintf(stderr,
                        "WARNING: return number %d is out of range of three bits\n",
                        temp_i);
            LASPoint_SetNumberOfReturns(point, temp_i);
/*          point->number_of_returns_of_given_pulse = temp_i & 7; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /*  // we expect the number of the return */
        else if (p[0] == 'r')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 7)
                fprintf(stderr,
                        "WARNING: return number %d is out of range of three bits\n",
                        temp_i);
/*          point->return_number = temp_i & 7; */
            LASPoint_SetReturnNumber(point, temp_i);
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the classification */
        else if (p[0] == 'c')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 255)
                fprintf(stderr,
                        "WARNING: classification %d is out of range of unsigned char\n",
                        temp_i);
            LASPoint_SetClassification(point, temp_i);
/*          point->classification = (unsigned char)temp_i; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the user data */
        else if (p[0] == 'u')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 255)
                fprintf(stderr,
                        "WARNING: user data %d is out of range of unsigned char\n",
                        temp_i);

            LASPoint_SetUserData(point, temp_i);
/*              point->user_data = temp_i & 255; */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the point source ID */
        else if (p[0] == 'p')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 65535)
                fprintf(stderr,
                        "WARNING: point source ID %d is out of range of unsigned short\n",
                        temp_i);
/*          point->point_source_ID = temp_i & 65535; */
            LASPoint_SetPointSourceId(point, temp_i);
            /*// then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the edge of flight line flag */
        else if (p[0] == 'e')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 1)
                fprintf(stderr,
                        "WARNING: edge of flight line flag %d is out of range of boolean flag\n",
                        temp_i);
            LASPoint_SetFlightLineEdge(point, temp_i ? 1: 0);
/*          point->edge_of_flight_line = (temp_i ? 1 : 0); */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the direction of scan flag */
        else if (p[0] == 'd')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            if (temp_i < 0 || temp_i > 1)
                fprintf(stderr,
                        "WARNING: direction of scan flag %d is out of range of boolean flag\n",
                        temp_i);
            LASPoint_SetScanDirection(point, temp_i ? 1: 0);
/*          point->scan_direction_flag = (temp_i ? 1 : 0); */
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the gps time */
        else if (p[0] == 't')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%lf", gps_time) != 1) return FALSE;
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the red channel of the RGB field */
        else if (p[0] == 'R')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            LASColor_SetRed(color, temp_i);
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the green channel of the RGB field */
        else if (p[0] == 'G')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            LASColor_SetGreen(color, temp_i);
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }
        /* // we expect the blue channel of the RGB field */
        else if (p[0] == 'B')
        {
            /* // first skip white spaces */
            while (l[0] && (l[0] == ' ' || l[0] == ',' || l[0] == '\t')) l++;
            if (l[0] == 0) return FALSE;
            if (sscanf(l, "%d", &temp_i) != 1) return FALSE;
            LASColor_SetBlue(color, temp_i);
            /* // then advance to next white space */
            while (l[0] && l[0] != ' ' && l[0] != ',' && l[0] != '\t') l++;
        }

        else
        {
        fprintf(stderr, "ERROR: next symbol '%s' unknown in parse control string\n", p);
        }
        p++;
    }
    LASPoint_SetColor(point, color);
    LASColor_Destroy(color);
    return TRUE;
}

int main(int argc, char *argv[])
{
    if (argc!=2)
    {
        fprintf(stderr,"Just provide the mdl, nothing else...\n");
        exit(-1);
    }

    std::string file_name_in    =   argv[1];
    std::string file_name_out   =   ncc::changeFileExt(file_name_in, compressed? "laz" : "las" );

    int i;
    int dry     = FALSE;
    int verbose = FALSE;
    double xyz_min[3] = {0.0, 0.0, 0.0};
    double xyz_max[3] = {0.0, 0.0, 0.0};
    double xyz_scale[3] = {0.01,0.01,0.01};
    double xyz_offset[3] = {0.0,0.0,0.0};
    unsigned int number_of_point_records = 0;
    unsigned int number_of_points_by_return[8] = {0,0,0,0,0,0,0,0};
    const char* parse_string = "xyzRGB";
    int file_creation_day = 0;
    int file_creation_year = 0;
    char* system_identifier = 0;
    char* generating_software = 0;
#define MAX_CHARACTERS_PER_LINE 512
    char line[MAX_CHARACTERS_PER_LINE];
    double xyz[3];
    LASPointH point = NULL;
    double gps_time;
    char* parse_less = NULL;
    LASHeaderH header = NULL;
    LASWriterH writer = NULL;
    LASError err;
    int format = LAS_FORMAT_12;

    int xyz_min_quant[3] = {0, 0, 0};
    int xyz_max_quant[3] = {0, 0, 0};

    double xyz_min_dequant[3] = {0.0, 0.0, 0.0};
    double xyz_max_dequant[3] = {0.0, 0.0, 0.0};


/*
    for (i = 1; i < argc; i++)
    {
        if (    strcmp(argv[i],"-h") == 0       ||
                strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   strcmp(argv[i],"-v") == 0   ||
                    strcmp(argv[i],"--verbose") == 0
            )
        {
            verbose = TRUE;
        }
        else if (   strcmp(argv[i],"-d") == 0       ||
                    strcmp(argv[i],"-dry") == 0     ||
                    strcmp(argv[i],"--dry") == 0
            )
        {
            dry = TRUE;
        }
        else if (   strcmp(argv[i],"--parse") == 0  ||
                    strcmp(argv[i],"-parse") == 0   ||
                    strcmp(argv[i],"-p") == 0
                )
        {
            i++;
            parse_string = argv[i];
        }
        else if (   strcmp(argv[i],"--scale") == 0  ||
                    strcmp(argv[i],"-scale") == 0   ||
                    strcmp(argv[i],"-s") == 0
                )
        {

            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[2]));
            xyz_scale[0] = xyz_scale[1] = xyz_scale[2];
        }
        else if (   strcmp(argv[i],"--xyz_scale") == 0  ||
                    strcmp(argv[i],"-xyz_scale") == 0
                )

        {
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[0]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[1]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_scale[2]));
        }
        else if (   strcmp(argv[i],"--xyz_offset") == 0  ||
                    strcmp(argv[i],"-xyz_offset") == 0
                )
        {
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[0]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[1]));
            i++;
            sscanf(argv[i], "%lf", &(xyz_offset[2]));
        }
        else if (   strcmp(argv[i],"--input") == 0  ||
                    strcmp(argv[i],"-input") == 0   ||
                    strcmp(argv[i],"-i") == 0       ||
                    strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            file_name_in = argv[i];
        }

        else if (   strcmp(argv[i],"--output") == 0  ||
                    strcmp(argv[i],"--out") == 0     ||
                    strcmp(argv[i],"-out") == 0     ||
                    strcmp(argv[i],"-o") == 0
                )
        {
            i++;
            file_name_out = argv[i];
        }

        else if (   strcmp(argv[i],"--format") == 0   ||
                    strcmp(argv[i],"-f") == 0    ||
                    strcmp(argv[i],"-format") == 0
                )
        {
            i++;
            if (strcmp(argv[i], "1.0") == 0) {
                format = LAS_FORMAT_10;
            }
            else if (strcmp(argv[i], "1.1") == 0) {
                format = LAS_FORMAT_11;
            }
            else if (strcmp(argv[i], "1.2") == 0) {
                format = LAS_FORMAT_12;
            }
            else {
                LASError_Print("Format must be specified as 1.0, 1.1, or 1.2");
            }

        }
        else if (   strcmp(argv[i],"--system_identifier") == 0   ||
                    strcmp(argv[i],"-system_identifier") == 0   ||
                    strcmp(argv[i],"-s") == 0   ||
                    strcmp(argv[i],"-sys_id") == 0)
        {
            i++;
            system_identifier = (char*) malloc(31 * sizeof(char));
            strcpy(system_identifier, argv[i]);
        }

        else if (   strcmp(argv[i],"--generating_software") == 0   ||
                    strcmp(argv[i],"-generating_software") == 0   ||
                    strcmp(argv[i],"-g") == 0   ||
                    strcmp(argv[i],"-gen_soft") == 0)
        {
            i++;
            generating_software = (char*) malloc(31*sizeof(char));
            strcpy(generating_software, argv[i]);
        }

        else if (   strcmp(argv[i],"--file_creation") == 0   ||
                    strcmp(argv[i],"-file_creation") == 0)
        {
            i++;
            file_creation_day = (unsigned short)atoi(argv[i]);
            i++;
            file_creation_year = (unsigned short)atoi(argv[i]);
        }
        else if (   file_name_in == NULL &&
                    file_name_out == NULL
                )
        {
            file_name_in = argv[i];
        }
        else if (   file_name_in &&
                    file_name_out == NULL
                )
        {
            file_name_out = argv[i];
        }
        else
        {
            fprintf(stderr, "ERROR: unknown argument '%s'\n",argv[i]);
            usage();
            exit(1);
        }
    }
*/

    ncc::RTd                    ptransform;
    std::vector<ncc::v3f4ub>    pdata;
    loadMDL(file_name_in , ptransform , pdata);

    /* create a cheaper parse string that only looks for 'x' 'y' 'z' and 'r' */
    parse_less = LASCopyString(parse_string);
    for (i = 0; i < (int)strlen(parse_string); i++)
    {
        if (parse_less[i] != 'x' &&
            parse_less[i] != 'y' &&
            parse_less[i] != 'z' &&
            parse_less[i] != 'r')
        {
            parse_less[i] = 's';
        }
    }

    do
    {
        parse_less[i] = '\0';
        //printf("nuking %d for %c\n", i, parse_less[i]);
        i--;
    } while (parse_less[i] == 's');


    /* first pass to figure out the bounding box and number of returns */
    if (verbose) {
        fprintf(stderr,
                "first pass over file '%s' with parse '%s'\n",
                file_name_in.c_str(),
                parse_less);
    }

//! START
    ncc::v3f pmin,pmax;
    pmin = pmax = pdata[0].pos();

    for (size_t i=0;i<pdata.size();++i)
    {
        pmin = ncc::min(pmin,pdata[i].pos());
        pmax = ncc::max(pmax,pdata[i].pos());
    }

    ncc::v3d pmind = pmin;
    ncc::v3d pmaxd = pmax;

    xyz_min[0] = pmind.x + ptransform[3].x;
    xyz_min[1] = pmind.y + ptransform[3].y;
    xyz_min[2] = pmind.z + ptransform[3].z;

    xyz_max[0] = pmaxd.x + ptransform[3].x;
    xyz_max[1] = pmaxd.y + ptransform[3].y;
    xyz_max[2] = pmaxd.z + ptransform[3].z;

    number_of_point_records = pdata.size();

    number_of_points_by_return[0] = pdata.size();
//!END

    /* did we manage to parse a line? */
    if (number_of_point_records == 0)
    {
        fprintf(stderr, "EEERROR: could not parse any lines with '%s'\n",
                parse_less);
        exit(1);
    }

    /* output some stats */
    if (verbose)
    {
        fprintf(stderr,
                "npoints %d min %g %g %g max %g %g %g\n",
                number_of_point_records,
                xyz_min[0],
                xyz_min[1],
                xyz_min[2],
                xyz_max[0],
                xyz_max[1],
                xyz_max[2]
                );
        fprintf(stderr,
                "return histogram %d %d %d %d %d %d %d %d\n",
                number_of_points_by_return[0],
                number_of_points_by_return[1],
                number_of_points_by_return[2],
                number_of_points_by_return[3],
                number_of_points_by_return[4],
                number_of_points_by_return[5],
                number_of_points_by_return[6],
                number_of_points_by_return[7]
                );
    }

    /*  compute bounding box after quantization */
    for (i = 0; i < 3; i++)
    {
        xyz_min_quant[i] = (int)(0.5 + (xyz_min[i] - xyz_offset[i]) / xyz_scale[i]);
        xyz_max_quant[i] = (int)(0.5 + (xyz_max[i] - xyz_offset[i]) / xyz_scale[i]);
    }

    for (i = 0; i < 3; i++)
    {
        xyz_min_dequant[i] = xyz_offset[i] + (xyz_min_quant[i] * xyz_scale[i]);
        xyz_max_dequant[i] = xyz_offset[i] + (xyz_max_quant[i] * xyz_scale[i]);
    }

    /* make sure there is not sign flip */
    for (i = 0; i < 3; i++)
    {
        if ((xyz_min[i] > 0) != (xyz_min_dequant[i] > 0))
        {
            fprintf(stderr,
                    "WARNING: quantization sign flip for %s min coord %g -> %g. use offset or scale up\n",
                    (i ? (i == 1 ? "y" : "z") : "x"),
                    xyz_min[i],
                    xyz_min_dequant[i]
                   );
        }
        if ((xyz_max[i] > 0) != (xyz_max_dequant[i] > 0))
        {
            fprintf(stderr,
                    "WARNING: quantization sign flip for %s max coord %g -> %g. use offset or scale up\n",
                    (i ? (i == 1 ? "y" : "z") : "x"),
                    xyz_max[i],
                    xyz_max_dequant[i]
                   );
        }
    }

    /* populate the header */

    header = LASHeader_Create();

    if (system_identifier) LASHeader_SetSystemId(header, system_identifier);
    if (generating_software) LASHeader_SetSoftwareId(header, generating_software);
    LASHeader_SetCreationDOY(header, file_creation_day);
    LASHeader_SetCreationYear(header, file_creation_year);


    if (format == LAS_FORMAT_10) {
        LASHeader_SetVersionMinor(header, 0);
    } else if (format == LAS_FORMAT_11){
        LASHeader_SetVersionMinor(header, 1);
    } else if (format == LAS_FORMAT_12) {
        LASHeader_SetVersionMinor(header, 2);
    }

    if (strstr(parse_string,"t") && (strstr(parse_string, "R") || strstr(parse_string, "G") ||strstr(parse_string, "B") ) )
    {
        fprintf(stderr, "Setting point format to 3, overriding version to 1.2 -- RGB + time\n");
        LASHeader_SetDataFormatId(header, 3);
        LASHeader_SetVersionMinor(header, 2);
    }
    else if ((strstr(parse_string, "R") || strstr(parse_string, "G") ||strstr(parse_string, "B") ) )
    {
        fprintf(stderr, "Setting point format to 2, overriding version to 1.2 -- RGB\n");
        LASHeader_SetDataFormatId(header, 2);
        LASHeader_SetVersionMinor(header, 2);
    }
    else if (strstr(parse_string,"t"))
    {
        fprintf(stderr, "Setting point format to 1\n");
        LASHeader_SetDataFormatId(header, 1);
    }

    else
    {
        LASHeader_SetDataFormatId(header, 0);
    }
    LASHeader_SetPointRecordsCount(header, number_of_point_records);
    LASHeader_SetScale(header, xyz_scale[0], xyz_scale[1], xyz_scale[2]);
    LASHeader_SetOffset(header, xyz_offset[0], xyz_offset[1], xyz_offset[2]);
    LASHeader_SetMin(header, xyz_min_dequant[0], xyz_min_dequant[1], xyz_min_dequant[2]);
    LASHeader_SetMax(header, xyz_max_dequant[0], xyz_max_dequant[1], xyz_max_dequant[2]);
    LASHeader_SetPointRecordsByReturnCount(header, 0, number_of_points_by_return[1]);
    LASHeader_SetPointRecordsByReturnCount(header, 1, number_of_points_by_return[2]);
    LASHeader_SetPointRecordsByReturnCount(header, 2, number_of_points_by_return[3]);
    LASHeader_SetPointRecordsByReturnCount(header, 3, number_of_points_by_return[4]);
    LASHeader_SetPointRecordsByReturnCount(header, 4, number_of_points_by_return[5]);
    LASHeader_SetCompressed(header,compressed);

    /*
        because the output goes to a file we can do everything in a
        single pass and compute the header information along the way
    */

    /* open output file */
    printf("Creating file...\n");
    writer = LASWriter_Create(file_name_out.c_str(), header, LAS_MODE_WRITE);
    if (!writer) {
        LASError_Print("Could not open file for write mode ");
        exit(1);
    }

    if (verbose) {
        fprintf(stderr,
                "scanning %s with parse '%s' writing to %s\n",
                file_name_in.c_str() ,
                parse_string,
                file_name_out.c_str()
               );
    }


    /* read the first line */
    for (size_t i=0;i<pdata.size();++i)
    {
        point = LASPoint_Create();
        LASPoint_SetHeader(point, header);

        ncc::v3f    pos    = pdata[i].pos();
        ncc::v4ub   color  = pdata[i].color();

        sprintf(line,"%lf %lf %lf %d %d %d",ptransform[3].x+pos.x,ptransform[3].y+pos.y,ptransform[3].z+pos.z,color.x,color.y,color.z);

        if (parse(parse_string, line, xyz, point, &gps_time))
        {
            /* compute the quantized x, y, and z values */
            LASPoint_SetX(point, xyz[0]);
            LASPoint_SetY(point, xyz[1]);
            LASPoint_SetZ(point, xyz[2]);

            LASPoint_SetTime(point, gps_time);

            /* write the first point */
            err = LASWriter_WritePoint(writer, point);
            if (err) {
                LASError_Print("could not write point");
                exit(1);
            }

        }
        else
        {
            fprintf(stderr, "WARNING: cannot parse '%s' with '%s'. skipping ...\n",
                    line,
                    parse_string);
        }
        LASPoint_Destroy(point);
        point = NULL;
    }


    /* close up stuff */
    LASWriter_Destroy(writer);
    if (verbose)
    {
        fprintf(stderr, "done.\n");
    }


    return 0;
}

#if 0

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <iostream>
using namespace std;

#pragma pack(1)
struct PublicHeaderBlock {

    PublicHeaderBlock() {

        memset(this,0,sizeof(PublicHeaderBlock));

        File_Signature[0]   = 'L';
        File_Signature[1]   = 'A';
        File_Signature[2]   = 'S';
        File_Signature[3]   = 'F';

        Version_Major       = 1;
        Version_Minor       = 4;

        sprintf(System_Identifier   ,"%s","OTHER");
        sprintf(Generating_Software ,"%s","GeoTo3D");

        time_t t                    = time(NULL);
        tm* timePtr                 = localtime(&t);
        File_Creation_Day_of_Year   = timePtr->tm_yday;
        File_Creation_Year          = 1900 + timePtr->tm_year;

        /*
        cout << "seconds= " << timePtr->tm_sec << endl;
        cout << "minutes = " << timePtr->tm_min << endl;
        cout << "hours = " << timePtr->tm_hour << endl;
        cout << "day of month = " << timePtr->tm_mday << endl;
        cout << "month of year = " << 1 + timePtr->tm_mon << endl;
        cout << "year = " << 1900 + timePtr->tm_year << endl;
        cout << "weekday = " << timePtr->tm_wday << endl;
        cout << "day of year = " << timePtr->tm_yday << endl;
        cout << "daylight savings = " << timePtr->tm_isdst << endl;
        */

        Header_Size = sizeof(PublicHeaderBlock);

    }


    void setData() {

        //! The actual number of bytes from the beginning of the file to the first field of the first point record data field.
        //Offset_to_point_data = ;

        //Point_Data_Record_Format  = ;
        //Point_Data_Record_Length  = ;

        //X_scale_factor;
        //Y_scale_factor;
        //Z_scale_factor;
        //X_offset;
        //Y_offset;
        //Z_offset;

        //! The max and min data fields are the actual unscaled extents of the LAS point file data, specified in the coordinate system of the LAS data.
        //Max_X;
        //Min_X;
        //Max_Y;
        //Min_Y;
        //Max_Z;
        //Min_Z;

        //Number_of_point_records;
        //Number_of_points_by_return[15];
    }



    char                File_Signature[4];
    unsigned short      File_Source_ID;
    unsigned short      Global_Encoding;
    unsigned int        Project_ID_GUID_data_1;
    unsigned short      Project_ID_GUID_data_2;
    unsigned short      Project_ID_GUID_data_3;
    unsigned char       Project_ID_GUID_data_4[8];
    unsigned char       Version_Major;
    unsigned char       Version_Minor;
    char                System_Identifier[32];
    char                Generating_Software[32];
    unsigned short      File_Creation_Day_of_Year;
    unsigned short      File_Creation_Year;
    unsigned short      Header_Size;
    unsigned int        Offset_to_point_data;
    unsigned int        Number_of_Variable_Length_Records;
    unsigned char       Point_Data_Record_Format;
    unsigned short      Point_Data_Record_Length;
    unsigned int        Legacy_Number_of_point_records;
    unsigned int        Legacy_Number_of_points_by_return[5];
    double              X_scale_factor;
    double              Y_scale_factor;
    double              Z_scale_factor;
    double              X_offset;
    double              Y_offset;
    double              Z_offset;
    double              Max_X;
    double              Min_X;
    double              Max_Y;
    double              Min_Y;
    double              Max_Z;
    double              Min_Z;
    unsigned long long  Start_of_Waveform_Data_Packet_Record;
    unsigned long long  Start_of_first_Extended_Variable_Length_Record;
    unsigned int        Number_of_Extended_Variable_Length_Records;
    unsigned long long  Number_of_point_records;
    unsigned long long  Number_of_points_by_return[15];
};

#pragma pack(1)
struct PointData {

    PointData() {
        memset(this,0,sizeof(PointData));
    }

    unsigned int    X;
    unsigned int    Y;
    unsigned int    Z;
    unsigned short  Intensity;
    unsigned char   Return_Number_AND_Number_of_Returns_AND_Scan_Direction_Flag_AND_Edge_of_Flight_Line;
    unsigned char   Classification;
    char            Scan_Angle_Rank;
    unsigned char   User_Data;
    unsigned short  Point_Source_ID;
    unsigned short  Red;
    unsigned short  Green;
    unsigned short  Blue;
};

int main(int argc, char** argv) {

    PublicHeaderBlock tmp;

	return 0;
}

#endif

