/* File : biorbd_python.i */
%{
#define SWIG_FILE_WITH_INIT
#include "s2mMusculoSkeletalModel.h"
#ifdef _WIN32
// This is a hack because Eigen can't be dynamically compiled on Windows, while dlib needs consistency in compilation. 
// Please note that this can result in undefined behavior while using s2mMuscleOptimisation...
const int USER_ERROR__inconsistent_build_configuration__see_dlib_faq_1_ = 0;
const int DLIB_VERSION_MISMATCH_CHECK__EXPECTED_VERSION_19_10_0 = 0;
#endif
%}

%include "numpy.i"
%init %{
    import_array();
%}


// typemaps.i is a built-in swig interface that lets us map c++ types to other
// types in our language of choice. We'll use it to map Eigen matrices to
// Numpy arrays.
%include <typemaps.i>
%include <std_vector.i>

/*** s2mJoints ***/
%typemap(typecheck) s2mJoints &{
    void *argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mJoints,  0  | 0)) && argp1) {
        // Test if it is a pointer to SWIGTYPE_p_s2mJoints already exists
        $1 = true;
    } else {
        $1 = false;
    }
}
%typemap(in) s2mJoints &{
    void * argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mJoints,  0  | 0)) && argp1) {
        // Recast the pointer
        $1 = reinterpret_cast< s2mJoints * >(argp1);
    } else {
        PyErr_SetString(PyExc_ValueError, "s2mJoints must be a s2mJoints");
        SWIG_fail;
    }
}

/*** s2mGenCoord ***/
%typemap(typecheck) s2mGenCoord &{
    std::cout << "typemap(typecheck) s2mGenCoord" << std::endl;
    void *argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mGenCoord,  0  | 0)) && argp1) {
        // Test if it is a pointer to SWIGTYPE_p_s2mGenCoord already exists
        $1 = true;
    } else if( PyArray_Check($input) ) {
        // test if it is a numpy array
        $1 = true;
    } else {
        $1 = false;
    }
}
%typemap(in) s2mGenCoord &{
    std::cout << "typemap(in) s2mGenCoord" << std::endl;
    void * argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mGenCoord,  0  | 0)) && argp1) {
        // Recast the pointer
        $1 = reinterpret_cast< s2mGenCoord * >(argp1);
    } else if( PyArray_Check($input) ) {
        // Get dimensions of the data
        int        ndim     = PyArray_NDIM    ((PyArrayObject*)$input);
        npy_intp*  dims     = PyArray_DIMS    ((PyArrayObject*)$input);

        // Dimension controls
        if (ndim != 1 ){
            PyErr_SetString(PyExc_ValueError, "s2mGenCoord must be a numpy vector");
            SWIG_fail;
        }

        // Get the data
        void*      data    = PyArray_DATA    ((PyArrayObject*)$input);
        unsigned int nQ(dims[0]);
        $1 = new s2mGenCoord(nQ);
        int typ=PyArray_TYPE((PyArrayObject*)$input);
        if (typ == NPY_LONG){
            for (unsigned int q = 0; q<nQ; ++q)
                (*$1)[q] = (double)(((int*)data)[q]);
        }
        else if(typ == NPY_DOUBLE)
            for (unsigned int q = 0; q<nQ; ++q)
                (*$1)[q] = ((double*)data)[q];
        else{
            PyErr_SetString(PyExc_ValueError, "Only int or double are allowed");
            SWIG_fail;
        }

    } else {
        PyErr_SetString(PyExc_ValueError, "s2mGenCoord must be a s2mGenCoord or numpy vector");
        SWIG_fail;
    }
};
%extend s2mGenCoord{
    PyObject* get_array(){
        int nQ($self->size());
        int nArraySize(1);
        npy_intp * arraySizes = new npy_intp[nArraySize];
        arraySizes[0] = nQ;

        double * q = new double[nQ];
        for (unsigned int i=0; i<nQ; ++i){
            q[i] = (*$self)(i);
        }
        PyObject* output = PyArray_SimpleNewFromData(nArraySize,arraySizes,NPY_DOUBLE, q);
        PyArray_ENABLEFLAGS((PyArrayObject *)output, NPY_ARRAY_OWNDATA);
        return output;
    };
}

/*** s2mMarkers ***/
%typemap(typecheck) s2mMarkers &{
    std::cout << "typemap(typecheck) s2mMarkers" << std::endl;
    void *argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mMarkers,  0  | 0)) && argp1) {
        // Test if it is a pointer to SWIGTYPE_p_s2mMarkers already exists
        $1 = true;
    } else {
        $1 = false;
    }
}
%typemap(in) s2mMarkers &{
    std::cout << "typemap(in) s2mMarkers" << std::endl;
    void * argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mMarkers,  0  | 0)) && argp1) {
        // Recast the pointer
        $1 = reinterpret_cast< s2mMarkers * >(argp1);
    } else {
        PyErr_SetString(PyExc_ValueError, "s2mMarkers must be a s2mMarkers");
        SWIG_fail;
    }
}


/*** s2mNodeBone ***/
%typemap(typecheck) s2mNodeBone &{
    void *argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mNodeBone,  0  | 0)) && argp1) {
        // Test if it is a pointer to SWIGTYPE_p_s2mNodeBone already exists
        $1 = true;
    } else if( PyArray_Check($input) ) {
        // test if it is a numpy array
        $1 = true;
    } else {
        $1 = false;
    }
}
%typemap(in) s2mNodeBone &{
    void * argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mNodeBone,  0  | 0)) && argp1) {
        // Recast the pointer
        $1 = reinterpret_cast< s2mNodeBone * >(argp1);
    } else if( PyArray_Check($input) ) {
        // Get dimensions of the data
        int        ndim     = PyArray_NDIM    ((PyArrayObject*)$input);
        npy_intp*  dims     = PyArray_DIMS    ((PyArrayObject*)$input);

        // Dimension controls
        if (ndim != 1 && (dims[0] < 3 || dims[0] > 4)){
            PyErr_SetString(PyExc_ValueError, "s2mNodeBone must be a numpy 3d vector");
            SWIG_fail;
        }

        // Get the data
        void*      data    = PyArray_DATA    ((PyArrayObject*)$input);
        $1 = new s2mNodeBone();
        int typ=PyArray_TYPE((PyArrayObject*)$input);
        if (typ == NPY_LONG){
            for (unsigned int i = 0; i<3; ++i)
                (*$1)[i] = (double)(((int*)data)[i]);
        }
        else if(typ == NPY_DOUBLE)
            for (unsigned int i = 0; i<3; ++i)
                (*$1)[i] = ((double*)data)[i];
        else{
            PyErr_SetString(PyExc_ValueError, "Only int or double are allowed");
            SWIG_fail;
        }

    } else {
        PyErr_SetString(PyExc_ValueError, "s2mNodeBone must be a s2mNodeBone or numpy vector");
        SWIG_fail;
    }
};
%extend s2mNodeBone{
    PyObject* get_array(){
        int nArraySize(1);
        npy_intp * arraySizes = new npy_intp[nArraySize];
        arraySizes[0] = 3;

        double * node = new double[3];
        for (unsigned int i=0; i<3; ++i){
            node[i] = (*$self)(i);
        }
        PyObject* output = PyArray_SimpleNewFromData(nArraySize,arraySizes,NPY_DOUBLE, node);
        PyArray_ENABLEFLAGS((PyArrayObject *)output, NPY_ARRAY_OWNDATA);
        return output;
    }
};


/*** s2mPath ***/
%typemap(typecheck) s2mPath &{
    void *argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mPath,  0  | 0)) && argp1) {
        // Test if it is a pointer to s2mPath already exists
        $1 = true;
    } else if( PyUnicode_Check($input) ) {
        // test if it is a string (python3 has unicode)
        $1 = true;
    } else {
        $1 = false;
    }
};
%typemap(in) s2mPath &{
    void * argp1 = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr($input, &argp1, SWIGTYPE_p_s2mPath,  0  | 0)) && argp1) {
        // Recast the pointer
        $1 = reinterpret_cast< s2mPath * >(argp1);
    } else if( PyUnicode_Check($input) ) {
        // Interpret the string
        $1 = new s2mPath(PyUnicode_AsUTF8($input));
    } else {
        PyErr_SetString(PyExc_ValueError, "s2mPath must be a s2mPath or string");
        SWIG_fail;
    }
};

%include ../biorbd.i



