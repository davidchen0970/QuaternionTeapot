#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <GL/glut.h>


// Class representing a 3D vertex
class Vertex3f {
public:
	float x, y, z;
	Vertex3f(float x, float y, float z) : x(x), y(y), z(z) {}

	// Friend function for overloading << operator
	friend std::ostream& operator<<(std::ostream& os, const Vertex3f& v3f) {
		os << "(" << v3f.x << ", " << v3f.y << ", " << v3f.z << ")";
		return os;
	}
};

// Definition of a Quaternion class
class Quaternion {
public:
	float w, x, y, z;

	// Constructor to initialize Quaternion with given values
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	// Constructor to create a Quaternion from axis-angle representation
	Quaternion(Vertex3f v3f, double angle) {
		w = cos(angle / 2);
		x = sin(angle / 2) * v3f.x;
		y = sin(angle / 2) * v3f.y;
		z = sin(angle / 2) * v3f.z;
	}

	// Overloaded multiplication operator for Quaternion multiplication
	Quaternion operator*(const Quaternion& other) const {
		return Quaternion(
			w * other.w - x * other.x - y * other.y - z * other.z,
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w
		);
	}

	// Calculate the inverse of the Quaternion
	Quaternion inverse() {
		float lenSq = w * w + x * x + y * y + z * z;
		return Quaternion(w / lenSq, -x / lenSq, -y / lenSq, -z / lenSq);
	}

	// Calculate the length (magnitude) of the Quaternion
	float length() const {
		return sqrt(w * w + x * x + y * y + z * z);
	}

	// Check if the Quaternion is a unit Quaternion
	bool isUnit() const {
		return fabs(length() - 1) < 0.0001;
	}

	// Check if the Quaternion is an identity Quaternion
	bool isIdentityQuaternion() const {
		return (fabs(w - 1) < 0.001 && fabs(x) < 0.001 && fabs(y) < 0.001 && fabs(z) < 0.001);
	}

	// Calculate the conjugate of the Quaternion
	Quaternion conjugate() const {
		return Quaternion(w, -x, -y, -z);
	}

	// Normalize the Quaternion to make it a unit Quaternion
	Quaternion normalize() const {
		float len = length();
		if (len > 0.0001f) {
			return Quaternion(w / len, x / len, y / len, z / len);
		}
		else {
			// Return a default Quaternion if length is too small to avoid division by zero
			return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	// Rotate a given point using the Quaternion
	Vertex3f rotationToPosition(Vertex3f p) {
		Quaternion q = normalize();
		Vertex3f out(0, 0, 0);
		out.x = (1 - 2 * q.y * q.y - 2 * q.z * q.z) * p.x + (2 * q.x * q.y - 2 * q.w * q.z) * p.y + (2 * q.x * q.z + 2 * q.w * q.y) * p.z;
		out.y = (2 * q.x * q.y + 2 * q.w * q.z) * p.x + (1 - 2 * q.x * q.x - 2 * q.z * q.z) * p.y + (2 * q.y * q.z - 2 * q.w * q.x) * p.z;
		out.z = (2 * q.x * q.z - 2 * q.w * q.y) * p.x + (2 * q.y * q.z + 2 * q.w * q.x) * p.y + (1 - 2 * q.x * q.x - 2 * q.y * q.y) * p.z;
		return out;
	}

	// Friend function for overloading << operator
	friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat) {
		os << "(" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")";
		return os;
	}
};

// Global variables
float angle = 0;
Vertex3f a(1, 1, 0);
Vertex3f eye(3,3,10), center(0,0,0), up(0,1,0);
Vertex3f teapotPosition(3, 0, 0);

// Function to display text on the screen
void showText(GLfloat render_x, GLfloat render_y) {
	// Load identity matrix for a clean transformation state
	glLoadIdentity();

	// Set text color to black
	glColor3f(0.0, 0.0, 0.0);

	// Set the raster position for text rendering
	glRasterPos2f(render_x, render_y);

	// Text to be displayed
	char text[] = "Enter 'A', 'S', 'D', 'W', 'Q', 'E' to change the viewing angle.";

	// Loop through each character in the text and render it using GLUT's bitmap characters
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
}


// Function to display rotation axis information
void showangle(GLfloat render_x, GLfloat render_y, Vertex3f v3f)
{
	// Load identity matrix to reset transformations
	glLoadIdentity();

	// Set text color to black
	glColor3f(0.0, 0.0, 0.0);

	// Set the position for the text
	glRasterPos2f(render_x, render_y);

	// Convert the coordinates of the rotation axis to a formatted string
	char strValue[20];
	sprintf(strValue, "(%.1f, %.1f, %.1f)", v3f.x, v3f.y, v3f.z);

	// Create the text message with the rotation axis information
	char text[100];
	sprintf(text, "The center of rotation axis: %s", strValue);

	// Loop through each character in the text and render it using GLUT's bitmap characters
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
}


// Function to display eye position information
void showEyePosition(GLfloat render_x, GLfloat render_y, Vertex3f v3f) {
	// Load the identity matrix to reset transformations
	glLoadIdentity();

	// Set the text color to black
	glColor3f(0.0, 0.0, 0.0);

	// Set the raster position for displaying text
	glRasterPos2f(render_x, render_y);

	// Create a string representing the eye position with one decimal place
	char strValue[20];
	sprintf(strValue, "(%.1f, %.1f, %.1f)", v3f.x, v3f.y, v3f.z);

	// Create a text string to display the eye position
	char text[100];
	sprintf(text, "The position of eye: %s", strValue);

	// Loop through the characters in the text and display them using GLUT bitmap characters
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
}


// Function to display teapot position information
void showTeapotPosition(GLfloat render_x, GLfloat render_y, Vertex3f v3f) {
	// Reset the current matrix to identity
	glLoadIdentity();

	// Set text color to black
	glColor3f(0.0, 0.0, 0.0);

	// Set the raster position for the text
	glRasterPos2f(render_x, render_y);

	// Convert the Vertex3f coordinates to a formatted string
	char strValue[20];
	sprintf(strValue, "(%.1f, %.1f, %.1f)", v3f.x, v3f.y, v3f.z);

	// Create a text string with teapot position information
	char text[100];
	sprintf(text, "The position of teapot: %s (i, j, k, l, i, p)", strValue);

	// Loop through each character in the text and render it using GLUT bitmap characters
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
	}
}


// Function to handle display
void myDisplay(void)
{
	// Load identity matrix and set clear color
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set drawing color to black
	glColor3f(0.0, 0.0, 0.0);

	// Set up the view using gluLookAt
	glPushMatrix();
		gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

		// Draw coordinate axes
		glPushMatrix();
			glBegin(GL_LINE_STRIP);
				glVertex3f(-1000, 0, 0);
				glVertex3f(1000, 0, 0);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0, -1000, 0);
				glVertex3f(0, 1000, 0);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0, 0, -1000);
				glVertex3f(0, 0, 1000);
			glEnd();
		glPopMatrix();

		// Draw a line representing the rotation axis
		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
			glBegin(GL_LINE_STRIP);
				glVertex3f(-1000 * a.x, -1000 * a.y, -1000 * a.z);
				glVertex3f(1000 * a.x, 1000 * a.y, 1000 * a.z);
			glEnd();
		glPopMatrix();

		// Apply rotation using Quaternion and draw a wire teapot
		glPushMatrix();
			Quaternion q1(a, angle);
			q1 = q1.normalize();
		
			GLfloat rotationMatrix[16] = {
				1 - 2 * q1.y * q1.y - 2 * q1.z * q1.z, 2 * q1.x * q1.y - 2 * q1.w * q1.z, 2 * q1.x * q1.z + 2 * q1.w * q1.y, 0.0f,
				2 * q1.x * q1.y + 2 * q1.w * q1.z, 1 - 2 * q1.x * q1.x - 2 * q1.z * q1.z, 2 * q1.y * q1.z - 2 * q1.w * q1.x, 0.0f,
				2 * q1.x * q1.z - 2 * q1.w * q1.y, 2 * q1.y * q1.z + 2 * q1.w * q1.x, 1 - 2 * q1.x * q1.x - 2 * q1.y * q1.y, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
			glMultMatrixf(rotationMatrix);
			
			glTranslatef(teapotPosition.x, teapotPosition.y, teapotPosition.z);
			glutWireTeapot(3);
		glPopMatrix();
	glPopMatrix(); // Pop the initial matrix stack

	// Display text information
	showText(-35, 18);
	showangle(-35, 16, a);
	showEyePosition(-35, 14, eye);
	showTeapotPosition(-35, 12, teapotPosition);

	glFlush(); // Flush the OpenGL pipeline

}

// Idle function for continuous rotation
void idle() {
	angle+= 0.005;
	glutPostRedisplay();
}


// Function to handle window reshaping
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)	{
		glOrtho(-20.0, 20.0, -20.0 * (GLfloat)h / (GLfloat)w, 20.0 * (GLfloat)h / (GLfloat)w, -20.0, 20.0);
	}
	else {
		glOrtho(-20.0 * (GLfloat)w / (GLfloat)h, 20.0 * (GLfloat)w / (GLfloat)h, -20.0, 20.0, -20.0, 20.0);
	}
	glMatrixMode(GL_MODELVIEW);
	myDisplay();
}

// Initialization function
void myinit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
		showText(10, 27);

}

// Function to handle keyboard input
void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	// Adjusting the eye position
	case 'W':
	case 'w':
		eye.y += 1;
		break;
	case 'S':
	case 's':
		eye.y -= 1;
		break;
	case 'a':
	case 'A':
		eye.x -= 1;
		break;
	case 'd':
	case 'D':
		eye.x += 1;
		break;
	case 'q':
	case 'Q':
		eye.z -= 1;
		break;
	case 'e':
	case 'E':
		eye.z += 1;
		break;

	// Adjusting the teapot position
	case 'i':
	case 'I':
		teapotPosition.y += 0.5;
		break;
	case 'k':
	case 'K':
		teapotPosition.y -= 0.5;
		break;
	case 'l':
	case 'L':
		teapotPosition.x += 0.5;
		break;
	case 'J':
	case 'j':
		teapotPosition.x -= 0.5;
		break;
	case 'o':
	case 'O':
		teapotPosition.z += 0.5;
		break;
	case 'u':
	case 'U':
		teapotPosition.z -= 0.5;
		break;
	}
	glutPostRedisplay();  // Trigger a display update
}


// Function to handle special keys input
void specialFunc(GLint key, GLint x, GLint y)
{
	if (key == GLUT_KEY_UP) a.y += 0.2;
	if (key == GLUT_KEY_DOWN) a.y -= 0.2;
	if (key == GLUT_KEY_LEFT) a.x -= 0.2;
	if (key == GLUT_KEY_RIGHT) a.x += 0.2;
	if (key == GLUT_KEY_PAGE_UP) a.z += 0.2;
	if (key == GLUT_KEY_PAGE_DOWN) a.z -= 0.2;
	glutPostRedisplay();
}


// Main function
int main(int argc, char** argv) {
	// Testing quaternion operations
	Quaternion q1(1, 2, 3, 4);
	Quaternion q2(5, 6, 7, 8);
	std::cout << "q1(1, 2, 3, 4);   q2(5, 6, 7, 8);" << std::endl;

	// Length (magnitude) of quaternions
	std::cout << "Length: " << std::endl;
	std::cout << "Length of q1: " << q1.length() << ", is unit: " << q1.isUnit() << std::endl;
	std::cout << "Length of q2: " << q2.length() << ", is unit: " << q2.isUnit() << std::endl;

	// Normalized length of quaternions
	std::cout << "Normalize: " << std::endl;
	std::cout << "Length of normalize q1: " << q1.normalize().length() << ", is unit: " << q1.normalize().isUnit() << std::endl;
	std::cout << "Length of normalize q2: " << q2.normalize().length() << ", is unit: " << q2.normalize().isUnit() << std::endl;

	// Conjugate of quaternions
	Quaternion q3 = q1.conjugate();
	Quaternion q4 = q2.conjugate();
	std::cout << "Conjugate: " << std::endl;
	std::cout << "Conjugate of q1: " << q3 << ", is unit: " << q3.isUnit() << std::endl;
	std::cout << "Conjugate of q2: " << q4 << ", is unit: " << q4.isUnit() << std::endl;

	// Inverse of quaternions
	Quaternion q5 = q1.inverse();
	Quaternion q6 = q2.inverse();
	Quaternion q7 = q5 * q1;
	Quaternion q8 = q2 * q6;
	std::cout << "Inverse: " << std::endl;
	std::cout << "Inverse of q1: " << q5 << ", check calculation: " << q7 << std::endl;
	std::cout << "Inverse of q2: " << q6 << ", check calculation: " << q8 << std::endl;

	// Inverse again
	std::cout << "Inverse again of q1: " << q5.inverse() << std::endl;
	std::cout << "Inverse again of q2: " << q6.inverse() << std::endl;
	std::cout << std::endl;

	// Generating identity quaternion
	Quaternion identityQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
	std::cout << "Identity Quaternion is identity: " << identityQuaternion.isIdentityQuaternion() << std::endl;
	std::cout << "Identity Quaternion * Identity Quaternion: " << identityQuaternion * identityQuaternion << std::endl;
	std::cout << "Identity Quaternion * Inverse of Identity Quaternion: " << identityQuaternion * identityQuaternion.inverse() << std::endl;

	// Initializing OpenGL environment
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1360, 700);
	glutCreateWindow("simple");
	myinit();

	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);

	glutMainLoop();

	return 0;
}
