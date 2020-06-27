#include "base.h"

#define MAXPOP 100  // max size of population
#define TAMPOP 20   // size of population
#define QTYGRS 30 // quantity of grass

int gen = 0, day = 1; 
// duck = id, x, y, life, speed, vision, hungry
int duck[100][6];
// grass = id, x, y, status
int grass[16][3];

int amountducks = TAMPOP;
int amountgrass = QTYGRS;

int grassExist(int g)
{ 	//return the status of the grass
	return grass[g][2];
}

int duckExist(int t)
{//return the status of the duck
	if (duck[t][2] <= 0)
		return 0;
	return 1;
}

void gengrass()
{
    for (int i = 0; i < QTYGRS; i++)
    {                         // nr entre -10 e +10 -> rand() %20 - 10;
        grass[i][0] = (rand() % 1000); //x
		grass[i][1] = (rand() % 1000); //y
		grass[i][2] = 1; // status 
    }
}

void Draw(void) {

    //background colour
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    glClear(GL_COLOR_BUFFER_BIT);
 
 	glPointSize(10.0f); //point size
    glLineWidth(5.0f); //line width


  	glPointSize(3.0f); //point size

	
    glEnd();

	// Print generation to screen
	char genQty[30]={'\0'};
	sprintf(genQty, "Day: %d", day);// Text
	glColor3f(0.0,1.0,0.0);// Text color
	glRasterPos2f(0,980);// Print position
	// Print each char
	for(int j=0;j<30;j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, genQty[j]);
	}

	// Print help
	char help[30]={'\0'};
	sprintf(help, "Enter->Next / r->Restart");// Text
	glColor3f(0.0,1.0,0.0);// Text color
	glRasterPos2f(0,940);// Print position
	// Print each char
	for(int j=0;j<30;j++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, help[j]);
	}

	// Print ducks
	glPointSize(10.0f); //point size
    glBegin(GL_POINTS);
   	glColor3f(255.0f, 120.0f, 0.0f); //orange for ducks
    for (int j = 0; j < MAXPOP; j++)
    {                         
		if (duckExist(j)) //if the duck exist
		{
			glVertex2f(duck[j][0] , duck[j][1]);
		}
	}
    glEnd();

	// Print grass
	glPointSize(5.0f); //point size
    glBegin(GL_POINTS);
   	glColor3f(0.0f, 200.0f, 0.0f); //green for grass
    for (int i= 0 ; i < QTYGRS; i++)
    {                         
		if (grassExist(i)) //if the grass exist
			glVertex2f(grass[i][0], grass[i][1]);
	}
    glEnd();

	// Print quantity ducks and grass
	char amountAll[30]={'\0'};
	sprintf(amountAll, "Ducks: %d | Grass: %d", amountducks, amountgrass);// Text
	glColor3f(0.0,1.0,0.0);// Text color
	glRasterPos2f(0,900);// Print position
	// Print each char
	for(int j=0;j<30;j++){
	
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, amountAll[j]);
	}

    //send dates to draw
    glFlush();
}


int nextduck(int t)
{ //return the nextest duck of the t duck
	int next = -1; //the return
	int x, y, auxx = 1000, auxy = 1000;
	for (int i = 0; i < (amountducks - 1); i++)
	{
		if(duckExist(i) && i != t) //verify if exists and are different of the actual
		{
			//then take the how much they has to walk to the grass 		
			x = abs(duck[i][0] - duck[t][0]); 
			y = abs(duck[i][1] - duck[t][1]);

			if ((x + y) < (auxx + auxy)) //take the less distance
			{
				next = i;
				auxx = x;
				auxy = y;
			}
		}
	}	

	return next;
}

int whosdead()
{//return the first dead duck id
	for (int i = 0; i < amountducks; i++)
	{
		if(!duckExist(i))
		{
			return i;
		}
	}
	return amountducks;
}


void tryreproduce(int t)
{//try reproduce
	int nextest = nextduck(t);
	if (nextest == -1)
		return;
	int distanceNext = abs(duck[nextest][0] - duck[t][0]) + abs(duck[nextest][1] - duck[t][1]);
	//things necessary to reproduce
		//a duck next 5 distance from t
		//both ducks has 20 or more than hungry
		//lucky of 50% to mate
	if ((distanceNext <= 5) && (duck[nextest][5] >= 20 && duck[t][5] >= 20) && rand()%2)
	{
			//printf("next: X%d Y%d   Atual: X%d Y%d\n", duck[nextest][0], duck[nextest][1], duck[t][0], duck[t][1] );
			duck[t][5] -= 10;
			duck[nextest][5] -= 10;
			
			int soulduck = whosdead(); //soulduck are the int of a dead duck or the next possible duck avaliable :D

			duck[soulduck][0] = duck[t][0];
			duck[soulduck][1] = duck[t][1];
			duck[soulduck][2] = (duck[t][2] + duck[nextest][2])/2;
			duck[soulduck][3] = (duck[t][3] + duck[nextest][3])/2;
			duck[soulduck][4] = (duck[t][4] + duck[nextest][4])/2;
			duck[soulduck][5] = 10;
			amountducks++;
	}
	//things gonna happen when reproduce
		//both ducks loses 10 hungry
		//media of the parents and mutate 1%
		//chance to born some other duck based on hungry (like 10% for which 10 hungry)
		//add to amountducks

	return;
}

void initpop()
{
    for (int i = 0; i < TAMPOP; i++)
    {                        // nr entre -10 e +10 -> rand() %20 - 10;
        duck[i][0] = (rand() % 1000); //x random for all pop
		duck[i][1] = (rand() % 1000); //y random for all pop
		duck[i][2] = (rand() % 25 + 5); //life random between 5 and 20
		duck[i][3] = (rand() % 200 + 100); //speed random between 50 and 100
		duck[i][4] = (rand() % 200 + 100); //vision random between 100 and 150
		duck[i][5] = (rand() % 60 + 10); //hungry random between 5 and 20
	}
}

void eatGrass(int t, int g)
{//t = duck, g = grass
	if (grassExist(g))
	{	
		grass[g][2] = 0;
		duck[t][5] += 6;
		amountgrass--;
	}

	if (amountgrass == 1)
	{
		amountgrass = QTYGRS;	
		gengrass();
	}
}

int nextgrass(int t)
{ //t = the duck are searching the seed
	int next; //the return
	int x, y, auxx = 1000, auxy = 1000;
	for (int i = 0; i < QTYGRS; i++)
	{
		if(grassExist(i)) //verify if exists
		{
			//then take the how much they has to walk to the grass 		
			x = abs(grass[i][0] - duck[t][0]); 
			y = abs(grass[i][1] - duck[t][1]);

			if ((x + y) < (auxx + auxy)) //take the less distance
			{
				next = i;
				auxx = x;
				auxy = y;
			}
		}
	}	

	return next;
}

//allowing duck to move to someplace
void moveDuckGrass(int t, int g, int distx, int disty)
{//t = the duck, g = the nextest grass
	int distance = distx + disty;
	int speed = duck[t][3];
	//printf("distance: %d\n\n", distance);
	if (distance < speed)
	{
		duck[t][0] = grass[g][0];
		duck[t][1] = grass[g][1];
		return;
	}
	else
	{
		if (distx <= (speed/2) && disty > (speed/2))
		{//	go to the x and the rest walk in y
			int walky = abs(duck[t][0] - grass[g][0]);
					//       400   -     200     =    200  		
			duck[t][0] = grass[g][0];

			if (grass[g][1] > duck[t][1])
			{
				duck[t][1] += walky;	
			}
			else
			{
				duck[t][1] -= walky;
			}
		}
		else if (disty <= (speed/2) && distx > (speed/2))
		{//	go to the y and the rest walk in x
			int walkx = abs(duck[t][1] - grass[g][1]);  		
			duck[t][1] = grass[g][1];

			if (grass[g][0] > duck[t][0])
			{
				duck[t][0] += walkx;	
			}
			else
			{
				duck[t][0] -= walkx;
			}
		} 
		else 
		{
			if (grass[g][0] > duck[t][0])
			{
				duck[t][0] += speed/2;	
			}
			else
			{
				duck[t][0] -= speed/2;
			}

			if (grass[g][1] > duck[t][1])
			{
				duck[t][1] += speed/2;	
			}
			else
			{
				duck[t][1] -= speed/2;
			}
		}


		//if distance > speed
		//walks equal(half speed) x and y

	}
	return;
}

void hunger(int t)
{
	
	if (duck[t][5] > 0)
	{ //take a status of hungry
		duck[t][5] -= 1;
	}
	else
	{//take life
		duck[t][2] -= 3;
		if (duck[t][2] <= 0)
		{//if below zero, starve
			amountducks--;
			//printf("The duck %d is now ded :(\n", t);
		}
	}
	return;
}

void moveDuckRandom(int t)
{
	int dx = rand() %3 -1; 
	int dy = rand() %3 -1;
	int duckspeed = duck[t][3]; //the speed of the duck
	int walkx = 0, walky = 0;

	while (dx == 0 && dy == 0)
	{
		dx = rand() %3 -1; 
		dy = rand() %3 -1;
	}

	if (dx == 0) 
	{
		duck[t][1] = duck[t][1] + (duckspeed * dy); 
	}
	else if (dy == 0)
	{
		duck[t][0] = duck[t][0] + (duckspeed * dx);
	}
	else 
	{
		duck[t][0] = duck[t][0] + (duckspeed/2 * dx) ;
		duck[t][1] = duck[t][1] + (duckspeed/2 * dy) ;
	}

	//set limits to move
	if (duck[t][0] > 1000)
		duck[t][0] = 999;
	if (duck[t][1] > 1000)
		duck[t][1] = 999;
	if (duck[t][0] < 0)
		duck[t][0] = 1;
	if (duck[t][1] < 0)
		duck[t][1] = 1;	

}

//realize the routine with all ducks
void routineDuck()
{	// first see, after go to the grass, after eat, so than reproduce
	for (int i = 0; i < MAXPOP; i++) //do with all ducks
	{
		if (duckExist(i))
		{
			printf("ID: %d   X: %d Y :%d \nLife: %d Speed: %d Vision: %d Hungry: %d\n\n",i , duck[i][0], duck[i][1], duck[i][2], duck[i][3], duck[i][4], duck[i][5]);
			//printf("Duck: %d Life: %d\n\n", i, duck[i][2]);
			//localize the nextest grass
			int next = nextgrass(i);
			//printf("GrassNext X: %d  Y: %d  Status: %d\n", grass[next][0], grass[next][1], grass[next][2]);
			int distx = abs(grass[next][0] - duck[i][0]); //distance to x of grass
			int disty = abs(grass[next][1] - duck[i][1]); //distance to y of grass
			int distance = distx + disty;
			
			//is the nextest grass on the vision limit?
			if (distance <= duck[i][4]) 
			{	//true so go to the grass
				if (distance > 4) //try go to the grass
					moveDuckGrass(i, next, distx, disty);
				else //are in the grass, so eat
					eatGrass(i, next);   
			}	
			else 
			{  //dont see, go to some other place			
				moveDuckRandom(i);
			}

			hunger(i); //Take the hungry after which routine

			tryreproduce(i);
		}
		
	}
	day++;
}

void Keyboard(unsigned char key, int x, int y)
{

    //ENTER
    if (key == 13){
	 	routineDuck();
	 	glutPostRedisplay();
    }
	// r
	if(key == 114){
		//gen = 0;
		//TaxMut = 60;
		int i = 0;
		initpop();
	 	glutPostRedisplay();
	}
}

int main(int argc, char *argv[]){

	srand(time(NULL));

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // glutInitWduckowPosition((glutGet(GLUT_SCREEN_WIDTH) - 800) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 800) / 2);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("Patos evolutivos");

    initpop();
	gengrass();

    glutDisplayFunc(Draw);

    glutKeyboardFunc(Keyboard);
 
    gluOrtho2D(0, 1000, 0, 1000);

	glutMainLoop();

	return 0;
}
