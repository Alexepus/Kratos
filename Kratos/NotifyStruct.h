typedef struct _NOTIFY_COORD
{
NMHDR hdr;
double x;
double y;
int n_digitsX;
int n_digitsY;
} NOTIFY_COORD;


typedef struct _NOTIFY_ZOOM
{
NMHDR hdr;
double xBeg;
double xEnd;
double yBeg;
double yEnd;
} NOTIFY_ZOOM;
