#include <vector>
#include "mesh.h"

//THIS IS THE ONLY FILE YOU NEED TO MODIFY and SUBMIT!!! NOTHING ELSE!!!
//Please RENAME as described below and then send it to assignments.eisemann@gmail.com 
//(I will not use this email for any other purposes than collecting your files)
//You should send per student only ONE file (this one).
//Each file should be renamed using your studentids.
//E.g., Imagine two people worked together and their ids are 12 and 14, 
//then you should send two files called "12.h" and "14.h"
//Any deviation from this naming convention will lead to 0 points because the exercise is verified automatically!!!
//Good Luck!


//Global array to store mesh material properties and algorithmic parameters
std::vector<Vec3Df> Kd;//diffuse coefficient per vertex
std::vector<Vec3Df> Ks;//specularity coefficient per vertex
std::vector<float> Shininess;//exponent for phong and blinn-phong specularities
int ToonDiscretize=4;//number of levels in toon shading
float ToonSpecularThreshold = 0.49f;//threshold for specularity

//Mesh - will be filled and loaded outside.
Mesh MyMesh;


//Helper function that you can ignore!
void initStudentVariables()
{
	//this is needed so that your table has the same size as the number of vertices.
	//later, the vertex indices received in the functions correspond to the same location in your vector.
	//in other words, you can store some information per vertex here.
	Kd.resize(MyMesh.vertices.size(), Vec3Df(0.5,0.5,0.5));
	Ks.resize(MyMesh.vertices.size(), Vec3Df(0.5,0.5,0.5));
	Shininess.resize(MyMesh.vertices.size(), 3);
}


//for debugging purposes or variable changes (e.g., modify the toon threshold as done below)
//please notice that some keys are already in use!
void yourKeyboardFunction(unsigned char key)
{
	cout<<"Key not used so far, so executing your code!"<<endl;
	
	//recolor the mesh 
	switch(key)
	{
		case 't': 
			ToonSpecularThreshold-=0.001f;
		break;
		case 'T': 
			ToonSpecularThreshold+=0.001f;
		break;
		case 'd': 
			ToonDiscretize-=1;
			if (ToonDiscretize<=0)
				ToonDiscretize=1;
		break;
		case 'D': 
			ToonDiscretize+=1;
		break;
		
		//in case you want to add colors! - Not mandatory

		case 'r': //decrase diffuse Kd coefficient in the red channel by 0.01
		break;
		case 'R': //increase diffuse Kd coefficient in the red channel by 0.01
		break;
		case 'g'://same for green
		break;
		case 'G':
		break;
		case 'b'://same for blue
		break;
		case 'B':
		break;
	}
	
	cout<<"ToonSpecular"<<ToonSpecularThreshold<<endl;
	cout<<"Toon Discretization levels"<<ToonDiscretize<<endl;

}


//Debug function
Vec3Df debugColor(unsigned int index)
{	//this function you can use in any way you like!
	//e.g., for debugging purposes!
	return MyMesh.vertices[index].n;
	//or 
	//return Kd[index];
}


///////////////
///Shading
///////////////
//standard lambertian shading: Kd * dot(N,L), clamped to zero when negative. Where L is the light vector
//
Vec3Df diffuseOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, unsigned int index)
{
	//		L = normalized light-direction vector (lightPos - vertexPos)
	//		Kd = color property vector (with RGB)
	//		N = normal (thank god there is one thing obvious), vector orthogonal to curPoint/vertex in the surface
	//daFormula: D = Id*Kd*cos(angle) (wikipedia)
	//		Id = light property (RGB) we don't see this one :( we could add it though 3:P
	//		Kd = surface property (RGB) of current vertex
	//		cos(angle) = dot(L,N) (wikipedia) : substitution gives:
	//		D = Id*Kd*dot(L,N) where dot is non-negative
	Vec3Df L = (lightPos - vertexPos).unit(); //vector pointing from light source to current vertex (point that is calculated)
	float dot = Vec3Df::dotProduct(normal, L);

	if (dot < 0) dot = 0; //something with clamped to zero (that's why things are black, comment it out for fun :) )

	return Kd.at(index)*dot;
}


//Phong (!) Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
//Follow the course, only calculate Ks pow(dot(V,R),shininess), where V is the view vector and R is the Reflection vector of the light (like in pool billard computed from the LightPos, vertexPos and normal).
//When computing specularities like this, verify that the light is on the right side of the surface, with respect to the normal
//E.g., for a plane, the light source below the plane cannot cast light on the top, hence, there can also not be any specularity. 
Vec3Df phongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index)
{
	//vector pointing from light source to current vertex (point that is calculated)
	Vec3Df L = (lightPos - vertexPos).unit();
	Vec3Df N = normal.unit();

	Vec3Df R = (2 * N * Vec3Df::dotProduct(N, L) - L).unit();

	Vec3Df V = cameraPos;
	V.normalize();


	//if the angle between lightposition and surface normal
	// is more than 90 degrees
	if (Vec3Df::dotProduct(L, N) < 0) { //( cosine(angleBetweenTwoUnitVectors) === dotProduct(theTwoUnitVectors) )
		//then things should be unlit/black/ zero vector/RGB(0,0,0)
		return Vec3Df(0, 0, 0);
	}
	//Ks*pow(dot(V,R),shininess) (copied from above)
	return Ks.at(index)*pow(Vec3Df::dotProduct(V, R), Shininess.at(0));
}

//Blinn-Phong Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
//Be careful!!! The pseudo code does some additional modifications to the formula seen in the course
//Follow the course version and calculate ONLY Ks * pow(dot(N,H), shininess). The definition of H is given on the page above and in the course.
//The same test as before should be used
Vec3Df blinnPhongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index)
{
	// Halfway vector
	Vec3Df H = ((lightPos - vertexPos)+ cameraPos).unit();

	//dot product of Normal and halfway vector
	float dot = Vec3Df::dotProduct(normal, H);

	// for showing black
	if (dot < 0) {
		dot = 0;
	}


	// if negative, return 0 for black
	if (Vec3Df::dotProduct(normal.unit(), (lightPos - vertexPos)) < 0) { 
		return Vec3Df(0, 0, 0);
	}

	//formulae
	return Ks[index] * pow(Vec3Df::dotProduct(normal, H), Shininess.at(index));
}


///////////////
//Toon Shading
///////////////
//use the variable ToonDiscretize.
//Normal diffuse shading has values between 0 and Kd (the diffuse coefficient).
//In toon shading, these values are discretized.
//This means, there are N (ToonDiscretize) uniform intervals between 0 and Kd - in this example, we assume a single color channel, you should use the values from the vector Kd 
//Let c(0)=0, c(1) ...c(N), c(N+1)=Kd be the boundary values of these intervals.
//For a value v in [c(i), c(i+1)), the function should return (c(i)+c(i+1))/2.
//For v=Kd, return (c(N)+c(N+1))/2, else 0.
Vec3Df toonShadingNoSpecular(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, unsigned int index)
{
	//vector pointing from light source to current vertex (point that is calculated)
	Vec3Df L = (lightPos - vertexPos).unit();

	//Normal dot producted with L
	float dot = Vec3Df::dotProduct(normal, L);

	//for black
	if (dot < 0) {
		dot = 0;
	}

	//Discretize all uniform intervals with 4
	float d = floorf(dot * ToonDiscretize);


	float interval = 1.0f / ToonDiscretize;

	//step shown in instructions: (c(i)+c(i+1))/2
	dot = interval * d + (interval * 0.5f);


	return Kd[index] * dot;
}

//Toon shading specularity
//The goal is to add white highlights.
//If a channel of Blinn-Phong Specularity has a value bigger or equal to ToonSpecularThreshold, then set it to 1, else to 0.
Vec3Df toonShadingOnlySpecular(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index)
{
	//vector pointing from light source to current vertex (point that is calculated)
	Vec3Df L = (lightPos - vertexPos).unit();

	//normal 
	Vec3Df N = normal.unit();

	//dot product of L and N
	float dot = Vec3Df::dotProduct(L, N);

	// Black making stuff
	if (dot < 0) {
		return Vec3Df(0, 0, 0);
	}

	//halfway vector
	Vec3Df H = (cameraPos - (lightPos - vertexPos)).unit();

	//dot product halfway vector and the normal
	float channel = Vec3Df::dotProduct(H, normal.unit());


	// if it is smaller than specularity return 1, else to 0. 
	if (channel > ToonSpecularThreshold)
		channel = 1;
	else
		channel = 0;


	return Ks[index] * channel;
}


///////////////
///INTERACTION
///////////////
Vec3Df userInteractionSphere(const Vec3Df & selectedPos, const Vec3Df & camPos)
{
	//RETURN the new light position, defined as follows.
	//selectedPos is a location on the mesh. Use this location to place the light source to cover the location as seen from camPos.
	//Further, the light should be at a distance of 1.5 from the origin of the scene - in other words, located on a sphere of radius 1.5 around the origin.
	
	//radius of 1.5
	float r = 1.5;

	//Getting tha D
	Vec3Df D = (selectedPos - camPos).unit();

	//Formulae
	float dot = sqrt(pow(Vec3Df::dotProduct(D, camPos), 2) - camPos.getSquaredLength() + r*r);
	float b = -(Vec3Df::dotProduct(D, camPos));

	//calculative stuff
	float d1 = b + dot;
	float d2 = b - dot;


	float distance = (d1 > d2) ? d2 : d1;
	return (camPos + (D * distance));
}

Vec3Df userInteractionShadow(const Vec3Df & selectedPos, const Vec3Df & selectedNormal, const Vec3Df & lightPos)
{
	//RETURN the new light position such that the light towards the selectedPos is orthgonal to the normal at that location
	//--- in this way, the shading boundary will be exactly at this location.
	//there are several ways to do this, choose one you deem appropriate given the current light position
	//no panic, I will not judge what solution you chose, as long as the above condition is met.
	// calculate difference


	//vector pointing from light source to current vertex (point that is calculated)
	Vec3Df L = lightPos - selectedPos;

	//dotproduct of L and the Normal
	float dot = Vec3Df::dotProduct(L, selectedNormal);

	//calculate the new light position which is orthogonal to the normal at that location. 
	Vec3Df result = lightPos - dot * selectedNormal;

	return result;	
}

Vec3Df userInteractionSpecular(const Vec3Df & selectedPos, const Vec3Df & selectedNormal, const Vec3Df & lightPos, const Vec3Df & cameraPos)
{
	//RETURN the new light position such that a specularity (highlight) will be located at selectedPos, when viewed from cameraPos and lit from ligthPos.
	//please ensure also that the light is at a distance of 1 from selectedpos! If the camera is on the wrong side of the surface (normal pointing the other way),
	//then just return the original light position.
	//There is only ONE way of doing this!
	Vec3Df N = selectedNormal;
	Vec3Df S = -1 * selectedPos;
	Vec3Df dot = S - 2 * Vec3Df::dotProduct(S, N) * N;
	dot.normalize();
	return dot;
}