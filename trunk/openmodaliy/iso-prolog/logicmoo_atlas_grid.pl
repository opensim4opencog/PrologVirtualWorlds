:-include('logicmoo_utils_header.pl').

toFace(Agent,Channel,Orientation):-
      getPosition(Agent,XYZFrom),
      getPosition(Channel,XYZTo),
      pointsToQuaternon(XYZFrom,XYZTo,Orientation).


distanceOf(Agent,Here,Distance):-
      getPosition(Agent,Agent),getPosition(Here,Channel),
      pointsToDistance(Agent,Channel,Distance).


pointsToDistance(Agent,Channel,Distance):-
      pointsToVector(Agent,Channel,VX,VY,VZ),
      vectorToLength(VX,VY,VZ,Distance).

vectorToLength(VX,VY,VZ,Distance):-
      Distance is sqrt(VX*VX + VY*VY + VZ*VZ).

pointsToVector(list([float(X1),float(Y1),float(Z1)]),list([float(X2),float(Y2),float(Z2)]),VX,VY,VZ):-
      VX is X2 - X1,VY is Y2 - Y1, VZ is Z2 - Z1.


toFromToQuatAndPosList(list([float(X1),float(Y1),float(Z1)]),Channel,Rate,Orientation,PosList):-
      pointsToVector(list([float(X1),float(Y1),float(Z1)]),Channel,VX,VY,VZ),
      vectorToQuaternon(VX,VY,VZ,Orientation),
      vectorToPosList(X1,Y1,Z1,VX,VY,VZ,Rate,PosList).


pointsToQuaternon(Agent,Channel,Quad):-
      pointsToVector(Agent,Channel,VX,VY,VZ),
      vectorToQuaternon(VX,VY,VZ,Quad).

vectorToQuaternon(VX,VY,VZ,Quad):-
	   vectorToAngle(VX,VY,VZ,Angle,Latitude,Longitude),
	   anglesToQuaternon(Angle,Latitude,Longitude,Quad).

vectorToPosList(list([float(X1),float(Y1),float(Z1)]),VX,VY,VZ,Rate,PosList):-
      vectorToPosList(X1,Y1,Z1,VX,VY,VZ,Rate,PosList).
      
vectorToPosList(X1,Y1,Z1,VX,VY,VZ,Rate,PosList):-
       vectorToLength(VX,VY,VZ,Distance),
	 Time is Distance/Rate,
	 Dx is VX / Time,
	 Dy is VY / Time,
	 Dz is VZ / Time,
       vectorPlusVelocityToVectorList(Time,X1,Y1,Z1,Dx,Dy,Dz,PosList).


vectorPlusVelocityToVectorList(Time,X1,Y1,Z1,Dx,Dy,Dz,[Pos]):-Time<1,!,
      vectorPlusVelocityToVector(X1,Y1,Z1,Dx*Time,Dy*Time,Dz*Time,Pos).

vectorPlusVelocityToVectorList(Time,X1,Y1,Z1,Dx,Dy,Dz,[list([float(Vx),float(Vy),float(Vz)])|List]):-
      vectorPlusVelocityToVector(X1,Y1,Z1,Dx,Dy,Dz,list([float(Vx),float(Vy),float(Vz)])),
      Time0 is Time-1,vectorPlusVelocityToVectorList(Time0,Vx,Vy,Vz,Dx,Dy,Dz,List).


vectorPlusVelocityToVector(X1,Y1,Z1,Dx,Dy,Dz,list([float(Vx),float(Vy),float(Vz)])):-
	    Vx is X1 + Dx,Vy is Y1 + Dy,Vz is Z1 + Dz.

vectorToVelosity(VX,VY,VZ,Distance,Rate,list([float(Dx),float(Dy),float(Dz)]),Time):-
      Time is Distance/Rate,
      Dx is VX / Time,
      Dy is VY / Time,
      Dz is VZ / Time.
	    

vectorToAngle(VX,VY,VZ,Angle,Latitude,Longitude):-
      my_atan(VY,VX,Angle), 
      my_atan(VZ,VX,Latitude), 
      my_atan(VZ,VY,Longitude).

my_atan(Y,X,Angle):-X=\=0,!,Angle is atan(Y/X). 
my_atan(Y,_,Angle):-Angle is atan(Y*100000000000000).

anglesToQuaternon(Angle,Latitude,Longitude,list([float(Qx),float(Qy),float(Qz),float(Qw)])):-
      Sin_a    is sin( Angle / 2 ),
      Sin_lat  is sin( Latitude ),
      Sin_long is sin( Longitude ),
      Qx       is Sin_a * cos( Latitude ) * Sin_long,
      Qy       is Sin_a * Sin_lat,
      Qz       is Sin_a * Sin_lat * cos( Longitude ),
      Qw      is cos( Angle / 2 ).
      
/*
Is optimization of this:

      Sin_a    is sin( Angle / 2 )
      Cos_a    is cos( Angle / 2 )
      Sin_lat  is sin( Latitude )
      Cos_lat  is cos( Latitude )
      Sin_long is sin( Longitude )
      Cos_long is cos( Longitude )
      Qx       is Sin_a * Cos_lat * Sin_long
      Qy       is Sin_a * Sin_lat
      Qz       is Sin_a * Sin_lat * Cos_long
      Qw       is Cos_a.
*/

% ==================================================
% Going Places
% ==================================================

:-dynamic(object/3).
:-dynamic(subpart/2).
:-dynamic(location/2).
:-dynamic(room/1).
:-dynamic(door/2).

:-multifile(here/1).
:-dynamic(here/1).
:-dynamic(goto/1).


corpius(Channel,Agent,Id):-
      getActiveForCC(Channel,Agent,Id).

locationOfChar(Channel,Agent,Place):-
      getActiveForCC(Channel,Agent,Id),
      wfQuery(loc(Id,Place)).


is_contained_in(Thing1,Thing2) :-  
    location(Thing1,Thing2).

is_contained_in(Thing1,Thing2) :-
  location(Thing,Thing2),
  is_contained_in(Thing1,Thing).


list_things(Place,Set) :-  
  findall(Thing,location(Thing, Place),Things),
  sort(Things,Set).

list_connections(Place,Set) :-  
  findall(Thing,connect(Place, Thing),Things),
  sort(Things,Set).

end_of_file.



===============================================
C++ version
===============================================
QuatToMatrix(QUAT * quat, float m[4][4]){

float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 


// calculate coefficients
x2 = quat->x + quat->x; y2 = quat->y + quat->y; 
z2 = quat->z + quat->z;
xx = quat->x * x2; xy = quat->x * y2; xz = quat->x * z2;
yy = quat->y * y2; yz = quat->y * z2; zz = quat->z * z2;
wx = quat->w * x2; wy = quat->w * y2; wz = quat->w * z2;


m[0][0] = 1.0 - (yy + zz); m[1][0] = xy - wz;
m[2][0] = xz + wy; m[3][0] = 0.0;

m[0][1] = xy + wz; m[1][1] = 1.0 - (xx + zz);
m[2][1] = yz - wx; m[3][1] = 0.0;


m[0][2] = xz - wy; m[1][2] = yz + wx;
m[2][2] = 1.0 - (xx + yy); m[3][2] = 0.0;


m[0][3] = 0; m[1][3] = 0;
m[2][3] = 0; m[3][3] = 1;

===============================================
Java version
===============================================
public class QUAT { 
   public QUAT(){
   }
   float x=0;
   float y=0;
   float z=0;
   float w=0;
}

public static float[16] QuatToMatrix(QUAT quat){

float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 
float[16]  m;
   
   // calculate coefficients
   x2 = quat.x + quat.x; y2 = quat.y + quat.y; 
   z2 = quat.z + quat.z;
   xx = quat.x * x2; xy = quat.x * y2; xz = quat.x * z2;
   yy = quat.y * y2; yz = quat.y * z2; zz = quat.z * z2;
   wx = quat.w * x2; wy = quat.w * y2; wz = quat.w * z2;
   
   
   m[0] = 1.0 - (yy + zz); m[1] = xy - wz;
   m[2] = xz + wy; m[3] = 0.0;
   
   m[4] = xy + wz; m[5] = 1.0 - (xx + zz);
   m[6] = yz - wx; m[7] = 0.0;
   
   
   m[8] = xz - wy; m[9] = yz + wx;
   m[10] = 1.0 - (xx + yy); m[11] = 0.0;
   
   
   m[12] = 0; m[13] = 0;
   m[14] = 0; m[15] = 1;
   
   return m;
}


