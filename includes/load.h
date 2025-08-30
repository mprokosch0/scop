#ifndef LOAD_H

#define LOAD_H

# include "scop.h"

typedef GLuint (*CREATESHADERPROC)(GLenum type);
typedef void   (*SHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void   (*COMPILESHADERPROC)(GLuint shader);
typedef GLuint (*CREATEPROGRAMPROC)(void);
typedef void   (*ATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void   (*LINKPROGRAMPROC)(GLuint program);
typedef void   (*USEPROGRAMPROC)(GLuint program);
typedef void   (*GENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void   (*BINDVERTEXARRAYPROC)(GLuint array);
typedef void   (*GENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void   (*BINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void   (*BUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void   (*VERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void   (*ENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void   (*GETSHADERIVPROC)(GLuint shader,  GLenum pname, GLint *params);
typedef void   (*GETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void   (*GETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void   (*GETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void   (*DELETESHADERPROC)(GLuint shader);
typedef GLint  (*GETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void   (*UNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   (*UNIFORMLIPROC)();
typedef void   (*GENERATEMIPMAPPROC)();
typedef void   (*DELETEBUFFERSPROC)();
typedef void   (*DELETEVERTEXARRAYSPROC)();
typedef void   (*DELETEPROGRAMPROC)();

extern CREATESHADERPROC            glCreateShader;
extern SHADERSOURCEPROC            glShaderSource;
extern COMPILESHADERPROC           glCompileShader;
extern CREATEPROGRAMPROC           glCreateProgram;
extern ATTACHSHADERPROC            glAttachShader;
extern LINKPROGRAMPROC             glLinkProgram;
extern USEPROGRAMPROC              glUseProgram;
extern GENVERTEXARRAYSPROC         glGenVertexArrays;
extern BINDVERTEXARRAYPROC         glBindVertexArray;
extern GENBUFFERSPROC              glGenBuffers;
extern BINDBUFFERPROC              glBindBuffer;
extern BUFFERDATAPROC              glBufferData;
extern VERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
extern ENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern GETSHADERIVPROC             glGetShaderiv;
extern GETSHADERINFOLOGPROC        glGetShaderInfoLog;
extern GETPROGRAMIVPROC            glGetProgramiv;
extern GETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
extern DELETESHADERPROC            glDeleteShader;
extern GETUNIFORMLOCATIONPROC      glGetUniformLocation;
extern UNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
extern UNIFORMLIPROC               glUniform1i;
extern GENERATEMIPMAPPROC          glGenerateMipmap;
extern DELETEBUFFERSPROC           glDeleteBuffers;
extern DELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
extern DELETEPROGRAMPROC           glDeleteProgram;


int loadGLFunctions(void);

#endif