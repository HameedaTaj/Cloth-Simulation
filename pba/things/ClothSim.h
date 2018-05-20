//-------------------------------------------------------
//
//  ClothSim.C
//
//  PbaThing for ClothSim
//
//--------------------------------------------------------

//include files
#include "PbaThing.h"
#include <Vector.h>
#include "Color.h"
#include "DynamicalState.h"
#include "SoftBodyStateData.h"


#ifdef __APPLE__
  #include <OpenGL/gl.h>   // OpenGL itself.
  #include <OpenGL/glu.h>  // GLU support library.
  #include <GLUT/glut.h>
#else
  #include <GL/gl.h>   // OpenGL itself.
  #include <GL/glu.h>  // GLU support library.
  #include <GL/glut.h> // GLUT support library.
#endif

#include <iostream>

#define FACESIZE 12
#define ROWMAX 20
#define COLUMNMAX 20
#define PARTICLES 400


namespace pba{
    
    class ClothSimThing: public PbaThingyDingy
    {
    public:
        ClothSimThing(const std::string nam="ClothSimThing") : PbaThingyDingy(nam),unitVector(pba::Vector(0,1,0)),forceGravity(pba::Vector(0,0,0)),lastindex(0)
        { 
            std::cout<<name<< "constructed\n";
        };
    
    ~ClothSimThing(){};
    
    void Init(const std::vector<std::string>& args)
    { 
        lastindex=particle.add(PARTICLES);
        
        pba::Vector vel;
        pba::Color col; int k=0;
        //initialize particle position
        for (int i=0;i<sqrt(PARTICLES);i++)
        {
            for(int j=0;j<sqrt(PARTICLES);j++)
            {
                double x=i;
                double y=j;
                particle.set_pos(k,pba::Vector((x - sqrt(PARTICLES)/2) / 4.5,(y - sqrt(PARTICLES)/2) / 4.5,-3));
                k++;
            }
        }
        //set the velocity and color
        for (size_t i=0;i<particle.nb();i++)
        {
            vel= pba::Vector(0,0,0);
            particle.set_vel(i,vel);
            col=pba::Color(rand()%2 ,rand()%2,rand()%2,0);
            particle.set_ci(i,col);
        }
        cout<<"Total Number of particles:"<<particle.nb()<<"\n";
        for(size_t i=0;i<=particle.nb();i++)
        {
            cout<<"i"<<i<<"particle position:"<<particle.pos(i).X()<<","<<particle.pos(i).Y()<<","<<particle.pos(i).Z()<<"\n";
        }
        //compute distance between particles
        sd.computedistance(&particle);
         cout<<"connected pair array data:"<<"\n";

            for(int i=0;i<sd.getsizeConnectedpair();i++)
            {
                //cout<<"i:"<<i<<"inode:"<<sd.getInode(i)<<","<<"jnode:"<<sd.getJnode(i)<<"\n";

            }
    }
     
    void Display()
    {
        
        glPointSize(2.5);
	//Draw the points
        glBegin(GL_POINTS);
        
        for(unsigned int i=0;i<particle.nb();i++)
        {
            //glColor3f(particle.ci(i).X(),particle.ci(i).Y(),particle.ci(i).Z());//retrive color of each particle
            glVertex3f(particle.pos(i).X(),particle.pos(i).Y(),particle.pos(i).Z());//retrive position of each particle
        }
        glEnd();
        
        glColor3f(0,191,250);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Draw the Lines
        glBegin(GL_LINES);
    	for(int n=0;n<sd.getsizeConnectedpair();n++)
    	{
       	 	size_t i = sd.getInode(n);
        	size_t j = sd.getJnode(n);
		size_t len=sd.getLij(n);
        	pba::Vector p1 = particle.pos(i);
        	pba::Vector p2 = particle.pos(j);
		//set the force for each particle from neighbor
		particle.set_force(i,particle.force(i)+sd.structforce(&particle,i,j,len));
		particle.set_force(j,particle.force(j)+sd.structforce(&particle,j,i,len));
		//Draw the line
        	glVertex3d(p1.X(), p1.Y(), p1.Z());
        	glVertex3d(p2.X(), p2.Y(), p2.Z());
    	}
    	glEnd();
    };
    void solve()
    {
        pba::Vector posTemp,vel;
        //Leap Frog solver        
	for(size_t i=0;i<particle.nb();i++)
        {
	    vel=pba::Vector(0,0,0);
            particle.set_vel(i,vel);
            posTemp=particle.pos(i)+particle.vel(i)*(dt/2);
            particle.set_pos(i,posTemp);
          
	    //set vel
	    if(particle.pos(i).X()!=2&&particle.pos(i).Y()!=2){
	    //if(particle.pos(i).X()!=2){
            //if(i!=399 && i!= 0){
            particle.set_vel(i,(particle.vel(i)+(particle.force(i))*dt));//calcalate new velocity
	    }
	
            particle.set_pos(i,posTemp+particle.vel(i)*(dt/2));//calculate new position
      
        }    
    };
          
    void Usage()
    {
            PbaThingyDingy::Usage();
            
    }; 

    
    private:
        pba::DynamicalStateData particle;
    
        pba::Vector unitVector,force;
        pba::Vector forceGravity;
        int lastindex;
        
        
        pba::Vector axis;
        double theta;
        pba::Vector xP;
        pba::Vector nP;
        
        pba::Vector verts[8];
        pba::Vector collisionPos;
        pba::Vector reflectedVel;
        SoftBodyStateData sd;
        SoftBodyStateData st;
        
};
    
  pba::PbaThing ClothSim()
    {
        return PbaThing( new pba::ClothSimThing() );
    }
   

    
}



