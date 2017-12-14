#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <sstream>
#include <list>
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ImageStruct.h"
#include "ImageStreamIO.h"

#ifdef __cplusplus
} //extern "C"
#endif

namespace py = pybind11;

struct MyDatatype {
    enum Type : uint8_t {
        UINT8 = _DATATYPE_UINT8,
        INT8 = _DATATYPE_INT8,
        UINT16 = _DATATYPE_UINT16,
        INT16 = _DATATYPE_INT16,
        UINT32 = _DATATYPE_UINT32,
        INT32 = _DATATYPE_INT32,
        UINT64 = _DATATYPE_UINT64,
        INT64 = _DATATYPE_INT64,
        FLOAT = _DATATYPE_FLOAT,
        DOUBLE = _DATATYPE_DOUBLE,
        COMPLEX_FLOAT = _DATATYPE_COMPLEX_FLOAT,
        COMPLEX_DOUBLE = _DATATYPE_COMPLEX_DOUBLE,
        EVENT_UI8_UI8_UI16_UI8 = _DATATYPE_EVENT_UI8_UI8_UI16_UI8
    };
    static const std::vector<uint8_t> Size;

    Type atype;
    uint8_t asize;

    MyDatatype(Type atype) : atype(atype), asize(Size[atype]) { };
};

const std::vector<uint8_t> MyDatatype::Size({
        SIZEOF_DATATYPE_UINT8,
        SIZEOF_DATATYPE_INT8,
        SIZEOF_DATATYPE_UINT16,
        SIZEOF_DATATYPE_INT16,
        SIZEOF_DATATYPE_UINT32,
        SIZEOF_DATATYPE_INT32,
        SIZEOF_DATATYPE_UINT64,
        SIZEOF_DATATYPE_INT64,
        SIZEOF_DATATYPE_FLOAT,
        SIZEOF_DATATYPE_DOUBLE,
        SIZEOF_DATATYPE_COMPLEX_FLOAT,
        SIZEOF_DATATYPE_COMPLEX_DOUBLE,
        SIZEOF_DATATYPE_EVENT_UI8_UI8_UI16_UI8    
});

std::string MydatatypeToPyFormat(MyDatatype dt){
    switch(dt.atype){
        case MyDatatype::Type::UINT8:  return py::format_descriptor<uint8_t>::format();
        case MyDatatype::Type::INT8:   return py::format_descriptor<int8_t>::format();
        case MyDatatype::Type::UINT16: return py::format_descriptor<uint16_t>::format();
        case MyDatatype::Type::INT16:  return py::format_descriptor<int16_t>::format();
        case MyDatatype::Type::UINT32: return py::format_descriptor<uint32_t>::format();
        case MyDatatype::Type::INT32:  return py::format_descriptor<int32_t>::format();
        case MyDatatype::Type::UINT64: return py::format_descriptor<uint64_t>::format();
        case MyDatatype::Type::INT64:  return py::format_descriptor<int64_t>::format();
        case MyDatatype::Type::FLOAT:  return py::format_descriptor<float>::format();
        case MyDatatype::Type::DOUBLE: return py::format_descriptor<double>::format();
        // case MyDatatype::Type::COMPLEX_FLOAT: return py::format_descriptor<(std::complex<float>>::format();
        // case MyDatatype::Type::COMPLEX_DOUBLE: return py::format_descriptor<(std::complex<double>>::format();
        default: throw "Not implemented";
    }
}

MyDatatype PyFormatToMydatatype(std::string pf){
    if(pf == py::format_descriptor<uint8_t>::format())  
        return MyDatatype::Type::UINT8;
    if(pf == py::format_descriptor<int8_t>::format())   
        return MyDatatype::Type::INT8;
    if(pf == py::format_descriptor<uint16_t>::format()) 
        return MyDatatype::Type::UINT16;
    if(pf == py::format_descriptor<int16_t>::format())  
        return MyDatatype::Type::INT16;
    if(pf == py::format_descriptor<uint32_t>::format()) 
        return MyDatatype::Type::UINT32;
    if(pf == py::format_descriptor<int32_t>::format())  
        return MyDatatype::Type::INT32;
    if(pf == py::format_descriptor<uint64_t>::format()) 
        return MyDatatype::Type::UINT64;
    if(pf == py::format_descriptor<int64_t>::format())  
        return MyDatatype::Type::INT64;
    if(pf == py::format_descriptor<float>::format())    
        return MyDatatype::Type::FLOAT;
    if(pf == py::format_descriptor<double>::format())   
        return MyDatatype::Type::DOUBLE;
    // case MyDatatype::Type::COMPLEX_FLOAT: return py::format_descriptor<(std::complex<float>>::format();
    // case MyDatatype::Type::COMPLEX_DOUBLE: return py::format_descriptor<(std::complex<double>>::format();
    throw "Not implemented";
}

// Expose classes and methods to Python
PYBIND11_MODULE(pyImageStreamIO, m) {
    m.doc() = "";

    py::class_<MyDatatype> mydatatype(m, "Datatype");
    py::enum_<MyDatatype::Type>(mydatatype, "Type")
        .value("UINT8", MyDatatype::Type::UINT8)
        .value("INT8", MyDatatype::Type::INT8)
        .value("UINT16", MyDatatype::Type::UINT16)
        .value("INT16", MyDatatype::Type::INT16)
        .value("UINT32", MyDatatype::Type::UINT32)
        .value("INT32", MyDatatype::Type::INT32)
        .value("UINT64", MyDatatype::Type::UINT64)
        .value("INT64", MyDatatype::Type::INT64)
        .value("FLOAT", MyDatatype::Type::FLOAT)
        .value("DOUBLE", MyDatatype::Type::DOUBLE)
        .value("COMPLEX_FLOAT", MyDatatype::Type::COMPLEX_FLOAT)
        .value("COMPLEX_DOUBLE", MyDatatype::Type::COMPLEX_DOUBLE)
        .value("EVENT_UI8_UI8_UI16_UI8", MyDatatype::Type::EVENT_UI8_UI8_UI16_UI8)    
        .export_values();

    // IMAGE_KEYWORD interface
    py::class_<IMAGE_KEYWORD> (m, "Image_kw")
        .def(py::init([]() {
            return std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
        }))
        .def(py::init([](std::string name, int64_t numl, std::string comment) {
            if(name.size()>16)
                throw "name too long";
            if(comment.size()>80)
                throw "comment too long";
            auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
            std::copy(name.begin(), name.end(), kw->name);
            kw->type = 'L';
            kw->value.numl = numl;
            std::copy(comment.begin(), comment.end(), kw->comment);
            return kw;
        }),py::arg("name"),
           py::arg("numl"),
           py::arg("comment") = "")
        .def(py::init([](std::string name, double numf, std::string comment) {
            if(name.size()>16)
                throw "name too long";
            if(comment.size()>80)
                throw "comment too long";
            auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
            std::copy(name.begin(), name.end(), kw->name);
            kw->type = 'D';
            kw->value.numf = numf;
            std::copy(comment.begin(), comment.end(), kw->comment);
            return kw;
        }),py::arg("name"),
           py::arg("numf"),
           py::arg("comment") = "")
        .def(py::init([](std::string name, std::string valstr, std::string comment) {
            if(name.size()>16)
                throw "name too long";
            if(valstr.size()>16)
                throw "valstr too long";
            if(comment.size()>80)
                throw "comment too long";
            auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
            std::copy(name.begin(), name.end(), kw->name);
            kw->type = 'S';
            std::copy(valstr.begin(), valstr.end(), kw->value.valstr);
            std::copy(comment.begin(), comment.end(), kw->comment);
            return kw;
        }),py::arg("name"),
           py::arg("valstr"),
           py::arg("comment") = "")
        .def_readonly("name", &IMAGE_KEYWORD::name)
        .def_readonly("type", &IMAGE_KEYWORD::type)
        .def("__repr__", [](const IMAGE_KEYWORD &kw) {
            std::ostringstream tmp_str;
            switch(kw.type) {
                case 'L' : tmp_str << kw.value.numl; break;
                case 'D' : tmp_str << kw.value.numf; break;
                case 'S' : tmp_str << kw.value.valstr; break;
                default  : tmp_str << "Unknown format"; break;
            }
            return tmp_str.str();
        })  // TODO handle union
        .def_readonly("comment", &IMAGE_KEYWORD::comment);

    // IMAGE_METADATA interface
    py::class_<IMAGE_METADATA> (m, "Image_md")
        // .def(py::init([]() {
        //     return std::unique_ptr<IMAGE_METADATA>(new IMAGE_METADATA());
        // }))
        .def_readonly("name", &IMAGE_METADATA::name)
        .def_readonly("naxis", &IMAGE_METADATA::naxis)
        .def_readonly("size", &IMAGE_METADATA::size)
        .def_readonly("nelement", &IMAGE_METADATA::nelement)
        .def_readonly("atype", &IMAGE_METADATA::atype)
        .def_readonly("creation_time", &IMAGE_METADATA::creation_time)
        .def_readonly("last_access", &IMAGE_METADATA::last_access)
        .def_readonly("shared", &IMAGE_METADATA::shared)
        .def_readonly("status", &IMAGE_METADATA::status)
        .def_readonly("logflag", &IMAGE_METADATA::logflag)
        .def_readonly("sem", &IMAGE_METADATA::sem)
        .def_readonly("cnt0", &IMAGE_METADATA::cnt0)
        .def_readonly("cnt1", &IMAGE_METADATA::cnt1)
        .def_readonly("cnt2", &IMAGE_METADATA::cnt2)
        .def_readonly("write", &IMAGE_METADATA::write)
        .def_readonly("NBkw", &IMAGE_METADATA::NBkw);
        
    // IMAGE interface
    py::class_<IMAGE> (m, "Image", py::buffer_protocol())
        .def(py::init([]() {
            return std::unique_ptr<IMAGE>(new IMAGE());
        }))
        .def_readonly("used", &IMAGE::used)
        .def_readonly("memsize", &IMAGE::memsize)
        .def_readonly("md", &IMAGE::md)
        // .def_readonly("array", &IMAGE::array)  // TODO handle union
        .def_readonly("kw", &IMAGE::kw)
        .def_buffer([](IMAGE &img) -> py::buffer_info {
            MyDatatype dt(static_cast<MyDatatype::Type>(img.md->atype));
            std::string format = MydatatypeToPyFormat(dt);
            std::vector<ssize_t> shape(img.md->naxis);
            std::vector<ssize_t> strides(img.md->naxis);
            ssize_t stride = dt.asize;
            for(int8_t axis(img.md->naxis-1); axis>=0; --axis ){
                shape[axis] = img.md->size[axis];
                strides[axis] = stride;
                stride *= shape[axis];
            }

            return py::buffer_info(
                    img.array.UI8,  /* Pointer to buffer */
                    dt.asize,       /* Size of one scalar */
                    format,         /* Python struct-style format descriptor */
                    img.md->naxis,  /* Number of dimensions */
                    shape,          /* Buffer dimensions */
                    strides         /* Strides (in bytes) for each index */
                );

        });;

    m.def("createIm", [](std::string name, py::buffer b, uint8_t shared, uint16_t NBkw) {
        /* Request a buffer descriptor from Python */
        py::buffer_info info = b.request();

        uint8_t atype = PyFormatToMydatatype(info.format).atype;
        std::vector<uint32_t> ushape(info.ndim);
        std::copy(info.shape.begin(), info.shape.end(), ushape.begin());                

        auto img = std::unique_ptr<IMAGE>(new IMAGE());
        ImageStreamIO_createIm(img.get(), name.c_str(), info.ndim, ushape.data(), atype, shared, NBkw);

        return img;
    }, R"pbdoc(
          Create shared memory image stream

          Parameters:
            name   [in]:  the name of the shared memory file will be SHAREDMEMDIR/<name>_im.shm
            buffer [in]:  np.array of the image.
            shared [in]:  if true then a shared memory buffer is allocated.  If false, only local storage is used.
            NBkw   [in]:  the number of keywords to allocate.
          )pbdoc",
          py::arg("name"),
          py::arg("buffer"),
          py::arg("shared") = 1,
          py::arg("NBkw") = 1
          );
    m.def("read_sharedmem_image_toIMAGE", &ImageStreamIO_read_sharedmem_image_toIMAGE,
          "Read / connect to existing shared memory image stream");

}
