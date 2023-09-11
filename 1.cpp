// 图像采集->图像重建->图像处理和分析->2D/3D可视化
// GLAD是用来管理OpenGL的函数指针
//顶点数组对象：Vertex Array Object，VAO
//顶点缓冲对象：Vertex Buffer Object，VBO
//元素缓冲对象：Element Buffer Object，EBO 或 索引缓冲对象 Index Buffer Object，IBO
//3D坐标转为2D坐标的处理过程by图形渲染管线
//第一部分把你的3D坐标转换为2D坐标，第二部分是把2D坐标转变为实际的有颜色的像素
着色器 // shader

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
// glCreateShader 创建着色器
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);

glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);

unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);


// 4. 绘制物体
glUseProgram(shaderProgram);

//生成纹理
unsigned int texture;
glGenTextures(1, &texture);

GLM opengl的math库

//5个坐标系统
物体空间、
世界空间、
观察(视觉)空间 摄像机
裁剪空间、
屏幕空间

投影矩阵 glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
opengl 是右手坐标系
                +y
                |   -z
                |  /
                | /
    -x__________|/__________+x
               /|
              / |
             /  |
          +z    -y

//时间旋转
model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

//深度测试
glEnable(GL_DEPTH_TEST);

//glClear函数中指定DEPTH_BUFFER_BIT位来清除深度缓冲：
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

camera
需要 相机位置、相机方向、右轴、上轴
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

叉乘 cross  判定左和右，内和外
a 叉乘 b 得正 a在b的右侧(右手坐标系)
判断点在不在三角形内部

# 模型观察投影矩阵（MVP）
model view projection

glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f); 
- 视野范围(弧度)、视窗宽高比、近平面z坐标、远平面z坐标
Projection Matrix:    View---  Clip

// Camera matrix
glm::mat4 View = glm::lookAt(
    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space   位置坐标
    glm::vec3(0,0,0), // and looks at the origin    目标坐标
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)  Y轴向量
    );
View Matrix:              World ---  View(Camera) 

glm::mat4 Model = glm::mat4(1.0f);  Model Matrix:            Local---  World

glm::mat4 mvp = Projection * View * Model; 
- 模型矩阵通过位移、缩放、旋转将物体变换至世界坐标系中
- 观察矩阵把所有的世界坐标变换为相对于摄像机位置与方向的观察坐标



## 深度缓冲
- 缓冲中存储每个片段的深度（即”Z”值）；并且每次绘制片段之前要比较当前与先前片段的深度值，看谁离摄像机更近
- glEnable(GL_DEPTH_TEST);
// Accept fragment if it closer to the camera than the former one
- glDepthFunc(GL_LESS);

## 光照
- 所有光照计算都在片段着色器里进行

glut  glfw 关系


## 数组对象
-   unsigned int VBO, cubeVAO;
- 顶点缓冲对象（VBO）来存储顶点数据  - 顶点数组对象（VAO）来描述顶点数据的格式
- VAO相当于是对很多个VBO的引用
    glGenVertexArrays(1, &cubeVAO);  生成数量、指定数组存储生成的顶点数组对象名称
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 缓冲对象的类型、缓冲对象的名称
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 前三个参数与顶点着色器vs有关、是否被标准化、步长、受VBO影响，1：在不使用VBO的情况下，就是一个指针，指向的是需要上传到顶点数据指针，项目中通常在不使用VBO的情况下，绘制之前，执行glBindBuffer(GL_ARRAY_BUFFER, 0)，否则会导致数组顶点无效，界面无法显示；2：使用VBO的情况下，先要执行glBindBuffer(GL_ARRAY_BUFFER, 1)，如果一个名称非零的缓冲对象被绑定至GL_ARRAY_BUFFER目标（见glBindBuffer）且此时一个定点属性数组被指定了，那么pointer被当做该缓冲对象数据存储区的字节偏移量。并且，缓冲对象绑定（GL_ARRAY_BUFFER_BINDING）会被存为索引为index的顶点属性数组客户端状态；此时指针指向的就不是具体的数据了。因为数据已经缓存在缓冲区了。这里的指针表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。 

    glEnableVertexAttribArray(0);

- glGenVertexArrays 生成顶点数组对象名称，只是名称   
- glBindVertexArray
- glGenBuffers 生成顶点缓冲区对象名称 
- glBindBuffer 
- glBufferData  往gpu缓冲区写入数据
- glUniformMatrix4fv 通过一致变量（uniform修饰的变量）引用将一致变量值传入渲染管线，将矩阵作为参数，传递给着色器程序中的uniform变量
- glfwPollEvents - glfw定期与窗口系统交流 轮训或者等待
- glAttachShader - 附加一个 shader 对象到程序对象
- glLinkProgram(ID);
- glDeleteShader(fragment); -着色器对象是单独的对象，会一直存在于opengl上下文
- glUseProgram  调用时，OpenGL将使用指定的着色器程序进行后续渲染命令，直到使用glUseProgram设置另一个程序
- glDrawArrays 将顶点数据传递给着色器程序中的in变量，进行计算和渲染，内部实现主要是对OpenGL状态机中的顶点数组和缓冲区进行处理和渲染，以实现绘制指定数量的顶点 
- glfwSwapBuffers 作用是交换前后缓冲区，将绘制的结果显示在屏幕上 OpenGL中，绘制操作是在后缓冲区中进行的，而前缓冲区则是用于显示的

# 片段着色器中
- in变量是由顶点着色器输出的变量传递给片段着色器的，uniform变量是由CPU代码设置的全局变量传递给片段着色器的。
- FragmentsShader主要的作用是告诉GPU如何处理光照、阴影、遮挡、环境

##  缩放 Zoom 改变视野大小
-  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
- 修改投影矩阵







配置
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

/*VTK*/ 核心类
vtkProp //数据的可视化表达由vtkProp的子类负责 
//Prop内部通常还有两个对象，一个是Mapper对象，负责存放数据和渲染信息，另一个是Property（属性）对象，负责控制颜色、不透明度等参数。

vtkActor //场景中的几何数据，vtkActor用来代表一个整个的渲染场景
vtkActor2D  // 二维空间中数据
vtkVolume  //场景中的体数据
vtkImageActor //负责图像显示
vtkLight Lights //表现和处理光线，只用于3D，二维中不使用光线。存储了光源的位置和方向，以及颜色和强度
vtkCamera //在绘制过程中,用相机类控制被绘制到场场景中的物体的数量,相机类提供了一些方法控制相机的位置,同时相机类也控制透视投影和立体观察。
vtkImageActor
vtkMapper //映射器类经常和查找表类(vtkLookupTable)一起对几何体进行变换和绘制,该类也是图形模型和可视化流水线之间的接口。
vtkLookupTable //查找表类和颜色变换类(vtkColorTransferFunction)是vtkScalarsToColors子类,主要为映射的数据赋予不同的颜色
vtkRenderer  //绘制类管理图形引擎,继承自vtkViewport,窗口渲染器
vtkRenderWindow  //绘制窗口类 计算机窗口
vtkRenderWindowInteractor //负责监听鼠标、键盘和时钟消息，并通过VTK中的Command/Observer设计模式进行相应的处理
vtkTransform //场景中的许多对象，如Prop、光源Light、照相机Camera等都需要在场景中合理的放置，它们通过vtkTransform参数可以方便的控制对象的位置和方向

vtkNew // 模板类 vtkNew<> 更轻量的智能指针
vtkImageViewer2 // 2D图像显示类,可以设置slice，3维数据的方向，以及窗口大小、窗宽、窗位
vtkNamedColors // 颜色相关 getColor


观察者/命令模式 实现交互
//两种方法：1、实例化vtkCallbackCommand，将自己写的回调函数，设给其；2、从vtkCommand类派生出子类，重写Execute方法
vtkSmartPointer<vtkCallbackCommand> mouseCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    mouseCallback->SetCallback ( MyCallbackFunc );
    interactor->SetRenderWindow(viewer->GetRenderWindow());
    interactor->AddObserver(vtkCommand::LeftButtonPressEvent, mouseCallback);

    1、Execute() 是纯虚函数，所以从vtkCommand派生的类都必须实现这个方法。
        2、在主程序中实例化一个vtkCommand子类的对象以及调用相关的方法。 3、调用AddObserver() 函数监听感兴趣的事件

        vtkSmartPointer
        vtkNew 更加轻量，推荐在局部变量使用，全局还是建议用vtkSmartPointer
