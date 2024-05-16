#include <Python.h>

#define WGET_CMD     "wget http://192.168.31.134:8080/?action=snapshot -O /tmp/SearchFace.jpg"
#define SEARCHFACE_FILE "/tmp/SearchFace.jpg"

void face_init(void)
{
    Py_Initialize(); //初始化

    //当前路径添加到sys.path中
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));
}

void face_final(void)
{
    // 关闭Python解释器
    Py_Finalize();
}


double face_category (void)
{
    //将返回值转换为C类型
    double result = 0.0;    
    
    system(WGET_CMD);
    if (0 == access(SEARCHFACE_FILE, F_OK)) {
        printf("Error: file not found\n");
        return result;
    }

    //导入noface模块
    PyObject *pModule = PyImport_ImportModule("face");//face.py
    if (!pModule) 
    {
        PyErr_Print();
        printf("Error: failed to load face.py\n");
        goto FAILED_MODULE;
    }


    //获取alibaba_face函数对象
    PyObject *pFunc = PyObject_GetAttrString(pModule, "alibaba_face");
    if (!pFunc) 
    {
        PyErr_Print();
        printf("Error: failed to load alibaba_face\n");
        goto FAILED_FUNC;
    }


    //调用alibaba_face函数并获取返回值
    PyObject *pValue = PyObject_CallObject(pFunc, NULL);
    if (!pValue) 
    {
        PyErr_Print();
        printf("Error: function call failed\n");
        goto FAILED_VALUE;
    }



    if (!PyArg_Parse(pValue, "d", &result))
    {
        PyErr_Print();
        printf("Error: parse failed");
        goto FAILED_RESULT;
    }

    //打印返回值
    printf("result = %0.2lf\n", result);


    // 释放所有引用的Python对象
FAILED_RESULT:
    Py_DECREF(pValue);
FAILED_VALUE:    
    Py_DECREF(pFunc);
FAILED_FUNC:
    Py_DECREF(pModule);
FAILED_MODULE:


    return result;

}



