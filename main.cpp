#include <iostream>
#include<windows.h>
#include<math.h>
#include<vector>
#include<limits>
#include<chrono>
#include<list>


using namespace std;

//=======================All the drawing related code========================================================//

int screenwidth;
int screenheight;
HANDLE hconsole=GetStdHandle(STD_OUTPUT_HANDLE); //creates a handle to the window and as parameter i've given it the standard output handle
SMALL_RECT rectwindow;
CHAR_INFO *screenbuffer;//created a pointer to the screen buffer of type CHAR_INFO
auto tp1 = chrono::system_clock::now();

//Screen Buffer functions
int CreateConsole(int width,int height,int fontwidth,int fontheight){


screenwidth=width;
screenheight=height;
rectwindow={0,0,1,1}; //this defines the viewing rectangle of the screen buffer
SetConsoleWindowInfo(hconsole,true,&rectwindow); //why true here? pointers used be aware
COORD BufferEnd={(short)screenwidth,(short)screenheight}; //Defines the coordinates of a character cell in a console screen buffer.The last cell in the buffer in this case.

CONSOLE_FONT_INFOEX FontProperties;//this struct sets the properties of individual character on screen
FontProperties.cbSize=sizeof(FontProperties);
FontProperties.dwFontSize.X=fontwidth;
FontProperties.dwFontSize.Y=fontheight;  //Different properties being set according to the given parameters
FontProperties.FontFamily=FF_DONTCARE;
FontProperties.FontWeight==FW_NORMAL;

//now set the parameters to the different Buffer functions
SetConsoleActiveScreenBuffer(hconsole);
SetConsoleScreenBufferSize(hconsole,BufferEnd);
SetCurrentConsoleFontEx(hconsole,false,&FontProperties); //why false here? pointers used be aware
rectwindow={0,0,(short)screenwidth-1,(short)screenheight-1};
SetConsoleWindowInfo(hconsole,true,&rectwindow); //why true here? pointers used be aware
screenbuffer=new CHAR_INFO[screenwidth*screenheight]; //an array of CHAR_INFO struct {still dont fully understand the new keyword
memset(screenbuffer,0,sizeof(CHAR_INFO)*screenwidth*screenheight);// this sets the memory bytes of screenbuffer to zero (all the bytes that encompass the area of the buffer)


return 1; //The function has to return true
}
enum COLOUR{
	FG_BLACK		= 0x0000,
	FG_DARK_BLUE    = 0x0001,
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY			= 0x0007, // Thanks MS :-/
	FG_DARK_GREY    = 0x0008,
	FG_BLUE			= 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED			= 0x000C,
	FG_MAGENTA		= 0x000D,
	FG_YELLOW		= 0x000E,
	FG_WHITE		= 0x000F,
	BG_BLACK		= 0x0000,
	BG_DARK_BLUE	= 0x0010,
	BG_DARK_GREEN	= 0x0020,
	BG_DARK_CYAN	= 0x0030,
	BG_DARK_RED		= 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW	= 0x0060,
	BG_GREY			= 0x0070,
	BG_DARK_GREY	= 0x0080,
	BG_BLUE			= 0x0090,
	BG_GREEN		= 0x00A0,
	BG_CYAN			= 0x00B0,
	BG_RED			= 0x00C0,
	BG_MAGENTA		= 0x00D0,
	BG_YELLOW		= 0x00E0,
	BG_WHITE		= 0x00F0,
};
enum PIXEL_TYPE{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};
void Draw(int x,int y,short c=0x2588,short col =0x000F){

if (x >= 0&&x<screenwidth&& y>=0 && y<screenheight)
 {
screenbuffer[y * screenwidth + x].Char.UnicodeChar = c;
screenbuffer[y * screenwidth + x].Attributes = col;
}
}
void DrawLine(int x1,int y1,int x2,int y2,short c = 0x2588, short col = 0x000F){
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
        if (dx >= 0)
        { x = x1; y = y1; xe = x2; }
        else
        { x = x2; y = y2; xe = x1;}
        Draw(x, y, c, col);
        for (i = 0; x<xe; i++)
        {
        x = x + 1;
        if (px<0)
        px = px + 2 * dy1;
        else
        {
        if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
        px = px + 2 * (dy1 - dx1);
        }
        Draw(x, y, c, col);
        }
		}
		else
		{
        if (dy >= 0)
        { x = x1; y = y1; ye = y2; }
        else
        { x = x2; y = y2; ye = y1; }
       Draw(x, y, c, col);

        for (i = 0; y<ye; i++)
        {
        y = y + 1;
        if (py <= 0)
        py = py + 2 * dx1;
            else
				{
    if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
    py = py + 2 * (dx1 - dy1);
     }
        Draw(x, y, c, col);
    }
    }
	}
void Clip(int &x, int &y){
if (x < 0) x = 0;
if (x >= screenwidth) x = screenwidth;
if (y < 0) y = 0;
if (y >= screenheight) y = screenheight;
}
void Fill(int x1,int y1,int x2,int y2, short c =0x2588, short col =0x000F){
Clip(x1, y1);
Clip(x2, y2);
for (int x = x1; x < x2; x++)
for (int y = y1; y < y2; y++)
Draw(x, y, c, col);
	}
void FillCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F){
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, c, col);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	};

//2D vector class and utility functions
struct vec2d{

float x;
float y;
float z;

vec2d(){

x=0;
y=0;
z=0;

}

vec2d(float a,float b){
x=a;
y=b;
}

};
vec2d vec_subtract(vec2d &a,vec2d &b){

vec2d c;

c.x=a.x-b.x;
c.y=a.y-b.y;

return c;
}
vec2d vec_add(vec2d &a,vec2d &b){

vec2d c;

c.x=a.x+b.x;
c.y=a.y+b.y;

return c;
}
vec2d vec_multiply(vec2d &a,float b){
vec2d c;
c.x=a.x*b;
c.y=a.y*b;
c.z=a.z*b;
return c;
}
vec2d vec_crossproduct(vec2d &a,vec2d &b){

vec2d c;

c.x=(a.y*b.z)-(a.z*b.y);
c.y=(a.z*b.x)-(a.x*b.z);
c.z=(a.x*b.y)-(a.y*b.x);

return c;

}
float vec_dotproduct(vec2d &a,vec2d &b){

float c=(a.x*b.x)+(a.y*b.y)+(a.z*b.z);
return c;
}
float vec_mag(vec2d &a){
float c=sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z));
return c;
}
vec2d vec_normalise(vec2d &a){
vec2d c;
float b;
b=vec_mag(a);
c.x=a.x/b;
c.y=a.y/b;
//zzzzZZc.z=a.z/b;

return c;
}
vec2d vec_triple_product(vec2d &a,vec2d &b){

vec2d c= vec_crossproduct(a,b);

return vec_crossproduct(c,b);

}
vec2d vec_triple_product_2(vec2d &a,vec2d &b,vec2d &c){

vec2d d=vec_crossproduct(a,b);
vec2d e=vec_crossproduct(d,c);

return e;

}


//===========================================A* Algorithm code starts here================================//

struct Node{

//Global goal and local goal
float localGoal = 100000000;
float GlobalGoal = 100000000;

//This will help to create and edit the visual grid that i make
int IsActive = 1;

//To get position in 2D space
int x=0;
int y=0;

//Parent
Node* parent = nullptr;

//Visited or not
bool IsVisited=false;

//Neighbours
vector<Node*>vec_neighbours;



Node(int a){IsActive=a;}
Node(){}

};



//create a grid of nodes

int tilewidth =20;
int tileheight=20;
int margin=10;

const int gridwidth=8;
const int gridheight=8;

Node grid[gridwidth*gridheight]={1, 1, 1, 1, 1, 1, 0, 0,
                                 1, 1, 0, 0, 0, 0, 0, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 0, 1, 0, 1, 1, 1,
                                 1, 1, 0, 1, 1, 0, 3, 1,
                                 1, 2, 0, 1, 0, 0, 1, 1,
                                 1, 1, 0, 1, 0, 1, 1, 1,
                                 1, 1, 1, 1, 0, 1, 1, 1};

Node* nodeStart = nullptr;
Node* nodeEnd = nullptr;

float distance(Node* a,Node* b){

return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));


}

void Solve_AStar(Node* ns, Node* ne){

  for(int x=0;x<gridwidth;x++){
      for(int y=0;y<gridheight;y++)
        {
            //some helpful variables
            int current = y*gridwidth+x;

            //Default all the nodes

             grid[current].IsVisited = false;
             grid[current].GlobalGoal = numeric_limits<float>::max();
             grid[current].GlobalGoal = numeric_limits<float>::max();
             grid[current].parent = nullptr;


            }
         }

         //Setup starting condition
         Node *nodeCurrent = ns;
         ns->localGoal = 0;
         ns->GlobalGoal = distance(ns,ne);

         //list
        list<Node*>listNotTestedNodes;
        listNotTestedNodes.push_back(ns);

        while(!listNotTestedNodes.empty())
        {
            //sort by global goal so lowest is first
            listNotTestedNodes.sort([](const Node* lhs, const Node* rhs){ return lhs->GlobalGoal < rhs->GlobalGoal; });

            while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->IsVisited==true)
            {
                listNotTestedNodes.pop_front();
            }

            if(listNotTestedNodes.empty()){break;}

            nodeCurrent = listNotTestedNodes.front();
            nodeCurrent->IsVisited = true;

             for(auto n :nodeCurrent->vec_neighbours)
             {

                 if(!n->IsVisited){
                    listNotTestedNodes.push_back(n);
                 }


                 float PossiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, n);

                 if(PossiblyLowerGoal < n->localGoal)
                 {

                    // n->IsActive=8;
                     n->parent = nodeCurrent;
                     n->localGoal = PossiblyLowerGoal;

                     n->GlobalGoal = n->localGoal + distance(n,ne);
                 }
             }



     }
}




int main()
{
   CreateConsole(200,200,3,3);




        //give nodes x and y positions=============================================================================

        for(int x=0;x<gridwidth;x++)
         {

            for(int y=0;y<gridheight;y++)
            {
                   //some helpful variables
                  int current = y*gridwidth+x;

                    //give x and y positions
                  grid[current].x= x*tilewidth+margin +5;
                  grid[current].y= y*tileheight+margin +5;


            }
         }









        //if the node is alive give it neighbors====================================================================================

                   for(int x=0;x<gridwidth;x++)
                         {

                            for(int y=0;y<gridheight;y++)
                            {

                                 //some helpful variables
                                      int current = y*gridwidth+x;
                                      int up  =  (y-1)*gridwidth+x;
                                      int down = (y+1)*gridwidth+x;
                                      int left =  y*gridwidth+(x-1);
                                      int topleft =  (y-1)*gridwidth+(x-1);
                                      int bottomleft =  (y+1)*gridwidth+(x-1);

                                      int right =  y*gridwidth+(x+1);
                                      int topright =  (y-1)*gridwidth+(x+1);
                                      int bottomright =  (y+1)*gridwidth+(x+1);


                                  if(grid[current].IsActive>0)
                                    {
                                        if(y>0 && grid[up].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[up]);}
                                        if(y<gridheight-1 && grid[down].IsActive > 0){ grid[current].vec_neighbours.push_back(&grid[down]); }
                                        if(x>0 && grid[left].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[left]);}
                                        if(x<gridwidth-1 && grid[right].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[right]);}

//                                        if(x<gridwidth-1 && y > 1 && grid[topright].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[topright]);}
//                                        if(x>1 && y > 1 && grid[topleft].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[topleft]);}
//                                        if(x<gridwidth-2 && y < gridheight-2 && grid[bottomright].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[bottomright]);}
//                                        if(x>1 && y < gridheight-1 && grid[bottomleft].IsActive > 0) {grid[current].vec_neighbours.push_back(&grid[bottomleft]);}

                                    }

                                    //pay attention to gridwidth-1 and gridheight-1 it is important!!!!!!!!

                            }
                         }






            //if node has an alive value of 2 or 3, store their address in start and End==========================================================

            for(int i=0;i<gridwidth;i++)
            {
                for(int j=0;j<gridheight;j++)
                {
                    if(grid[j*gridwidth+i].IsActive==2){ nodeStart = &grid[j*gridwidth+i];}
                    if(grid[j*gridwidth+i].IsActive==3){ nodeEnd = &grid[j*gridwidth+i];}

                }
            }





            // A* is only supposed to be called when we update something and not every frame
              Solve_AStar(nodeStart,nodeEnd);










while(1){

        //==========================timing here=========================================
        auto tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();


        Fill(0,0,screenwidth , screenheight,PIXEL_SOLID,FG_BLACK);

















            //Draw the links===========================================================================================================

        for(int i=0;i<gridwidth;i++)
        {
            for(int j=0;j<gridheight;j++)
            {

                  for( auto w : grid[j*gridwidth+i].vec_neighbours)
                    {

                         DrawLine(grid[j*gridwidth+i].x, grid[j*gridwidth+i].y, w->x ,w->y, PIXEL_SOLID, FG_DARK_BLUE);
                        // DrawLine(grid[j*gridwidth+i].x, grid[j*gridwidth+i].y, grid[j*gridwidth+i].vec_neighbours[k].x ,grid[j*gridwidth+i].vec_neighbours[k].y, PIXEL_SOLID, FG_DARK_BLUE);

                    }

            }
        }




         //Draw the nodes=========================================================================================================

        for(int x=0; x<gridwidth; x++)
            {
                for(int y=0;y<gridheight;y++)
                {
                    int current = y*gridwidth+x;
                    if(grid[current].IsActive > 0){Fill(x*tilewidth+margin,y*tileheight+margin,x*tilewidth+tilewidth,y*tileheight+tileheight,PIXEL_SOLID,FG_DARK_BLUE);}
                    if(grid[current].IsActive==0){Fill(x*tilewidth+margin,y*tileheight+margin,x*tilewidth+tilewidth,y*tileheight+tileheight,PIXEL_HALF,FG_GREY);}
                    if(grid[current].IsActive==8){Fill(x*tilewidth+margin,y*tileheight+margin,x*tilewidth+tilewidth,y*tileheight+tileheight,PIXEL_HALF,FG_DARK_GREEN);}
                    if(&grid[current]==nodeStart){Fill(x*tilewidth+margin,y*tileheight+margin,x*tilewidth+tilewidth,y*tileheight+tileheight,PIXEL_SOLID,FG_DARK_YELLOW);}
                    if(&grid[current]==nodeEnd){Fill(x*tilewidth+margin,y*tileheight+margin,x*tilewidth+tilewidth,y*tileheight+tileheight,PIXEL_SOLID,FG_DARK_RED);}

                }
            }







            if(nodeEnd != nullptr)
            {
                Node* p =nodeEnd;
                while(p->parent != nullptr)
                {
                    DrawLine(p->x, p->y, p->parent->x, p->parent->y ,PIXEL_SOLID, FG_DARK_YELLOW);

                    //set the next node to this node's parent
                    p = p->parent;
                }


            }
















    WriteConsoleOutputW(hconsole,screenbuffer,{(short)screenwidth,(short)screenheight},{0,0},&rectwindow);
    }




    return 0;
}
