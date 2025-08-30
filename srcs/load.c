
#include "../includes/load.h"

CREATESHADERPROC            glCreateShader;
SHADERSOURCEPROC            glShaderSource;
COMPILESHADERPROC           glCompileShader;
CREATEPROGRAMPROC           glCreateProgram;
ATTACHSHADERPROC            glAttachShader;
LINKPROGRAMPROC             glLinkProgram;
USEPROGRAMPROC              glUseProgram;
GENVERTEXARRAYSPROC         glGenVertexArrays;
BINDVERTEXARRAYPROC         glBindVertexArray;
GENBUFFERSPROC              glGenBuffers;
BINDBUFFERPROC              glBindBuffer;
BUFFERDATAPROC              glBufferData;
VERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
ENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
GETSHADERIVPROC             glGetShaderiv;
GETSHADERINFOLOGPROC        glGetShaderInfoLog;
GETPROGRAMIVPROC            glGetProgramiv;
GETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
DELETESHADERPROC            glDeleteShader;
GETUNIFORMLOCATIONPROC      glGetUniformLocation;
UNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
UNIFORMLIPROC               glUniform1i;
GENERATEMIPMAPPROC          glGenerateMipmap;
DELETEBUFFERSPROC           glDeleteBuffers;
DELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
DELETEPROGRAMPROC           glDeleteProgram;

int loadGLFunctions()
{
    glCreateShader = (CREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    glShaderSource = (SHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    glCompileShader = (COMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    glCreateProgram = (CREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    glAttachShader = (ATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    glLinkProgram = (LINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    glUseProgram = (USEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    glGenVertexArrays = (GENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (BINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glGenBuffers = (GENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    glBindBuffer = (BINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    glBufferData = (BUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    glVertexAttribPointer = (VERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (ENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    glGetShaderiv = (GETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (GETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
    glGetProgramiv = (GETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (GETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
    glDeleteShader = (DELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    glGetUniformLocation = (GETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (UNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");
    glUniform1i = (UNIFORMLIPROC)glfwGetProcAddress("glUniform1i");
    glGenerateMipmap = (GENERATEMIPMAPPROC)glfwGetProcAddress("glGenerateMipmap");
    glDeleteBuffers = (DELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
    glDeleteVertexArrays = (DELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    glDeleteProgram = (DELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    if (!glCreateShader || !glShaderSource || !glCompileShader) {
        fprintf(stderr, "Erreur: certaines fonctions OpenGL modernes ne sont pas dispo.\n");
        return 0;
    }
    return 1;
}
