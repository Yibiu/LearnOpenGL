# 第三章 OpenGL绘制方式

[TOC]



前面章节围绕OpenGL最简单程序简单介绍了OpenGL的资源以及初始化，这节将具体介绍OpenGL的绘制资源以及使用这些资源进行绘制时的绘制方式。具体的渲染命令还是要配合程序来理解才能更加透彻，建议先做大概了解。

## 一：OpenGL图元

图元即最简单的图像构成单元，例如一副图片的构成单元是一个像素。OpenGL可以绘制很多复杂的形状，把这些形状无限放大看，它们都是由图元构成。OpenGL的图元有 **点、线和三角形** 。

| 图元类型   | OpenGL枚举量      |
| ---------- | ----------------- |
| 点         | GL_PONITS         |
| 线         | GL_LINES          |
| 条带线     | GL_LINE_STRIP     |
| 循环线     | GL_LINE_LOOP      |
| 独立三角形 | GL_TRIANGLES      |
| 三角形条带 | GL_TRIANGLE_STRIP |
| 三角形扇面 | GL_TRIANGLE_FAN   |

相关操作：

```c++
void glPointSize(GLfloat size);
void glLineWidth(GLfloat width);

// 优化
void glPolygonMode(GLenum face, GLenum mode);
void glFrontFace(GLenum mode);
void glCullFace(GLenum mode);
```





## 二：顶点缓冲对象VBO

第一章已经大概介绍过VBO，这节将具体介绍。

### 2.1 创建与分配缓存

```c++
void glCreateBuffers(GLsizei n, GLuint *buffers);
void glBindBuffer(GLenum target, GLuint buffer);
```

### 2.2 向缓存输入输出数据

#### 2.2.1 拷贝

最直接方式：

```c++
void glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
```

其次，可以选择部分修改：

```c++
void glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void glClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
```

另外，buffer之间的相互拷贝：

```c++
void glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
```

回读数据：

```c++
void glGetNamedBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void *data);
```

#### 2.2.2 映射

上面介绍的函数都会造成OpenGL的一次数据拷贝，可以使用映射的方式将GPU和CPU内存映射，这样可以减少拷贝。

```c++
void* glMapBuffer(GLenum target, GLenum access);
GLboolean glUnmapNamedBuffer(GLuint buffer);
void *glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
void glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length);
```

### 2.3 丢弃缓存

缓存使用完毕后可以通知OpenGL丢弃缓存，将缓存用于他处：

```c++
void glInvalidateBufferData(GLuint buffer);
void glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr lenth);
```





## 三：顶点属性

### 3.1 顶点属性

缓存内容填充之后需要解释缓存中数据内容，这些就由顶点属性完成。

```c++
// 单精度
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
// 整型
void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
// 双精度
void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
```

详细的数据格式参考书中介绍。顶点属性默认是关闭的，使用之前需要激活。

### 3.2 静态顶点属性

当顶点属性并未使能时，使用的是静态顶点属性，即是顶点的默认属性。当有大量顶点使用相同资源时，我们可以设置静态顶点属性来避免内存浪费。

详细操作参考书中。





## 四：OpenGL绘制命令

### 4.1 基本绘制

OpenGL的绘制命令大体上分为两种：

- **非索引形式：** 将缓存对象中的顶点属性按照自身排列顺序直接取出并使用；

- **索引形式：** 使用了元素数组(EBO/IBO)缓存中的索引数据来索引各个顶点属性数组。

```c++
void glDrawArrays(GLenum mode, GLint first, GLsizei count);
void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
```

所有看起来更为复杂的OpenGL绘制函数，在本质上都是基于这两个函数来完成功能实现的。

其他绘制函数：

```c++
void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
void glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
void glDrawArraysIndirect(GLenum mode, const GLvoid *indirect);
void glDrawElementsIndirect(GLenum mode, GLenum type, const GLvoid *indirect);
void glMultiDrawArrays(GLenum mode, const GLint *first, const GLint *count, GLsizei primcount);
void glMultiDrawElements(GLenum mode, const GLint *count, GLenum type, const GLvoid* const *indices, GLsizei primcount);
void glMultiDrawElementsBaseVertex(GLenum mode, const GLint *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, const GLint *baseVertex);
void glMultiDrawArraysIndirect(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
void glMultiDrawElemensIndirect(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
```

### 4.2 图元重启

略

### 4.3 多实例渲染

略







