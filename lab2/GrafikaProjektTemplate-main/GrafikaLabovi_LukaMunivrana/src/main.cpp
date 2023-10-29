#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<string>
#include<ctype.h>


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main()
{
	float coordinates[9];
	float normals[3];

	for (int i = 0; i < 9; i += 3) {
		std::cout << "Enter x y z coordinates for " << i/3 + 1 << ". triangle peak: " << std::endl;
		std::cin >> coordinates[i] >> coordinates[i + 1] >> coordinates[i + 2];
	}

	for (int i = 0; i < 3; i++) {
		std::cout << "Enter " << i + 1 << ". indice: " << std::endl;
		std::cin >> normals[i];
	}

	std::ofstream outfile("GeoLik.obj");

	outfile << "g triangle \n" << std::endl;

	for (int i = 0; i < 9; i += 3) {
		outfile << "v " << coordinates[i] << " " << coordinates[i + 1] << " " << coordinates[i + 2] << std::endl;
	}

	outfile << std::endl << "f " << normals[0] << " " << normals[1] << " " << normals[2];
	outfile.close();
	
	std::ifstream file;
	file.open("triangles.obj");

	std::string stringf;

	int totalPeaks = 0;
	int totalTriangles= 0;

	// Count peaks and normals(triangles)
	while (std::getline(file, stringf)) {

		if (!stringf.compare(0, 1, "v"))
			totalPeaks++;

		else if (!stringf.compare(0, 1, "f"))
			totalTriangles++;

		std::cout << stringf << "\n";
	}

	GLfloat* vertices = new GLfloat[totalPeaks * 3 + 1];
	int sizeofvertices = totalPeaks * 3;
	GLuint* indices = new GLuint[totalTriangles * 3 + 1];
	int sizeofindices = totalTriangles * 3;

	//clear the buffer
	file.clear();

	//start reading file from beggining
	file.seekg(0, std::ios::beg);

	while (std::getline(file, stringf)) {

		int i = 1;
		int j = 0;
		int k = 0;
		std::string number = "";

		// Finding values for vertices
		if (!stringf.compare(0, 1, "v")) {

			while (i <= stringf.length()) {

				if (stringf[i] == ' ') {

					vertices[j] = stof(number);			// Buffer overrun

					std::cout << number << std::endl;
					number = "";
					j++;
				}

				else
					number = number + stringf[i];

				i++;
			}

			std::cout << number << std::endl;
			vertices[j] = stof(number);					// Buffer overrun
		}

		// Finding values for indices
		else if (!stringf.compare(0, 1, "f")) {

			while (i <= stringf.length()) {

				if (stringf[i] == ' ') {

					indices[k] = stoi(number);			// Buffer overrun

					std::cout << number << std::endl;
					number = "";
					k++;
				}

				else
					number = number + stringf[i];

				i++;
			}

			std::cout << number << std::endl;
			indices[k] = stoi(number);					// Buffer overrun
		}
	}

	// Initialize GLFW
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	gladLoadGL();

	glViewport(0, 0, 800, 800);




	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);


	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLuint VAO, VBO, EBO;


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeofvertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofindices * sizeof(GLuint), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, sizeofindices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	delete[] vertices;
	delete[] indices;




	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}