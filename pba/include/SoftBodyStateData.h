//
//  SoftBodyStateData.h
//  
//
//  Created by Hameeda dildar on 04/09/18.
//
#include <Vector.h>
#include <Matrix.h>
#include <iostream>
#include <LinearAlgebra.h>
#ifndef SoftBodyStateData_h
#define SoftBodyStateData_h

namespace pba {
    struct softEdge
    {
       
        int inode;
        int jnode;
        double Lij;//edge length
        
    };
    class SoftBodyStateData : public DynamicalStateData
    {
    public:
	std::vector<softEdge> connected_pairs;
        SoftBodyStateData()
        {
            ks=0.05;
            //kf=1;
            kf=1;
	    ksh=0.05;
            g=0.00000000098;
            
            std::cout<<"Softbody constructor called\n";
            
        }
        ~SoftBodyStateData(){};
        void computedistance(pba::DynamicalStateData *DS)
        {
            for(size_t i = 0; i < DS->nb(); i++)
            {
                int temp = (int)sqrt(DS->nb());
                if((int)i % temp != (temp - 1) && i < (DS->nb() - temp))
                {
                    softEdge se1;
                    se1.inode = i;
                    se1.jnode = i+1;
                    se1.Lij = (DS->pos(se1.inode) - DS->pos(se1.jnode)).magnitude();
                    connected_pairs.push_back(se1);
                    
                    softEdge se2;
                    se2.inode = i;
                    se2.jnode = i+temp;
                    se2.Lij = (DS->pos(se2.inode) - DS->pos(se2.jnode)).magnitude();
                    connected_pairs.push_back(se2);
                    
                    softEdge se3;
                    se3.inode = i;
                    se3.jnode = i+temp+1;
                    se3.Lij = (DS->pos(se3.inode) - DS->pos(se3.jnode)).magnitude();
                    connected_pairs.push_back(se3);
                    
                    softEdge se4;
                    se4.inode = i+1;
                    se4.jnode = i+temp;
                    se4.Lij = (DS->pos(se4.inode) - DS->pos(se4.jnode)).magnitude();
                    connected_pairs.push_back(se4);
                }
                else if((int)i % temp == (temp - 1) && i < (DS->nb() - temp))
                {
                    softEdge se;
                    se.inode = i;
                    se.jnode = i+temp;
                    se.Lij = (DS->pos(se.inode) - DS->pos(se.jnode)).magnitude();
                    connected_pairs.push_back(se);
                }
                else if((int)i % temp != (temp - 1) && i >= (DS->nb() - temp))
                {
                    softEdge se;
                    se.inode = i;
                    se.jnode = i+1;
                    se.Lij = (DS->pos(se.inode) - DS->pos(se.jnode)).magnitude();
                    connected_pairs.push_back(se);
                }
            }
        }
        void updateConnectedPair(int a,int b,double value)
        {
            softEdge s;
            s.inode=a;
            s.jnode=b;
            s.Lij=value;
            connected_pairs.push_back(s);
        }
        int getsizeConnectedpair()
        {
            return connected_pairs.size();
        }
        int getInode(int i)
        {
            softEdge s;
            s=connected_pairs[i];
            return s.inode;
            
        }
        int getJnode(int i)
        {
            softEdge s;
            s=connected_pairs[i];
            return s.jnode;
            
        }
     
        double getLij(int i)
        {
            softEdge s;
            s=connected_pairs[i];
            return s.Lij;
            
        }
	//compute forces
        pba::Vector computeg(pba::DynamicalStateData *DS,int i)
        {
           
            pba::Vector unitVector,forceGravity;
            unitVector = pba::Vector(0,1,0);
            forceGravity = ( -g*DS->mass(i)) *  unitVector;
            return forceGravity;
        }
        pba::Vector springforce(pba::DynamicalStateData *DS,int i,int j,double L)
        {
            pba::Vector Xi,Xj,Xij,fspring;
            Xi=DS->pos(i);
            Xj=DS->pos(j);
            Xij=(Xj-Xi)/(Xj-Xi).magnitude();
       
            fspring=ks*((Xi-Xj).magnitude()-L)*Xij;
            
            return fspring;
        }
	pba::Vector shearforce(pba::DynamicalStateData *DS,int i,int j,double L)
        {
            pba::Vector Xi,Xj,Xij,fspring;
            Xi=DS->pos(i);
            Xj=DS->pos(j);
            Xij=(Xj-Xi)/(Xj-Xi).magnitude();
       
            fspring=-ksh*((Xi-Xj).magnitude()-L)*Xij;
            
            return fspring;
        }
        pba::Vector frictionforce(pba::DynamicalStateData *DS,int i,int j)
        {
            pba::Vector Vi,Vj,Xi,Xj,Xij,frictionforce;
           
            Vi=DS->vel(i);
	    Vj=DS->vel(j);
	    Xi=DS->pos(i);
            Xj=DS->pos(j);
            Xij=(Xj-Xi)/(Xj-Xi).magnitude();
            
            frictionforce = kf * ((Vj - Vi) * Xij) * (Xj - Xi);
           
            return frictionforce;
        }
        pba::Vector structforce(pba::DynamicalStateData *DS,int i,int j,double L)
        {
              //fstruct=springforce(DS,i,j,L)+frictionforce(DS,i,j);
              //fstruct=springforce(DS,i,j,L);
	     //fstruct=frictionforce(DS,i,j);
	    pba::Vector tempforce;
	    if(L<0.3)
		{
		   tempforce+=springforce(DS,i,j,L);
		}
	    else
		{
		    tempforce+=shearforce(DS,i,j,L);
		}
	    fstruct=tempforce;
            return (fstruct+frictionforce(DS,i,j)+computeg(DS,i));
        }
      
    private:
        
        
        double ks,kf,ksh,g;
        pba::Vector fstruct,force;
    };
    
}

#endif /* SoftBodyStateData_h */
