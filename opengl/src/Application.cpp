#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/example/imgui_impl_glfw.h"
#include "vendor/imgui/example/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Debug.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //设置缓存交换间隔为1(显示器60hz即每秒交换60次)
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //顶点索引
    float positions[] = {
       -0.5f,-0.5f,0.0f,0.0f, //0
        0.5f,-0.5f,1.0f,0.0f, //1
        0.5f,0.5f,1.0f,1.0f,  //2
       -0.5f,0.5f,0.0f,1.0f  //3
    };

    //三角形生成索引
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    //Enable Blend
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //VertexArray 
    VertexArray vao;
    VertexBufferLayout layout;

    VertexBuffer vbo(positions, 4 * 4 * sizeof(float));
    IndexBuffer ibo(indices, 6);
  
    layout.Push<float>(2);
    layout.Push<float>(2);
    vao.AddBuffer(vbo, layout);

	//Texture
	Texture texture("res/textures/ChernoLogo.png");
	texture.Bind();

    //Shader
    Shader shader;
    shader.CreateShader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    shader.SetUniform1i("u_Texture", 0);

    //Renderer
    Renderer renderer;

    float r = 0.2f;
    float increment = 0.05f;

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        renderer.Clear();
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        renderer.Draw(vao, ibo, shader);

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        //int display_w, display_h;
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();



    glfwTerminate();
    return 0;
}