#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <sstream>
#include <list>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#include "ImageStruct.h"
#include "ImageStreamIO.h"

#ifdef __cplusplus
} //extern "C"
#endif

namespace py = pybind11;

struct CacaoDatatype
{
    enum Type : uint8_t
    {
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

    CacaoDatatype(uint8_t atype) : atype(static_cast<CacaoDatatype::Type>(atype)),
                                asize(Size[atype]){};
};

const std::vector<uint8_t> CacaoDatatype::Size({0,
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
                                             SIZEOF_DATATYPE_EVENT_UI8_UI8_UI16_UI8});

std::string CacaoDatatypeToPyFormat(CacaoDatatype dt)
{
    switch (dt.atype)
    {
    case CacaoDatatype::Type::UINT8:
        return py::format_descriptor<uint8_t>::format();
    case CacaoDatatype::Type::INT8:
        return py::format_descriptor<int8_t>::format();
    case CacaoDatatype::Type::UINT16:
        return py::format_descriptor<uint16_t>::format();
    case CacaoDatatype::Type::INT16:
        return py::format_descriptor<int16_t>::format();
    case CacaoDatatype::Type::UINT32:
        return py::format_descriptor<uint32_t>::format();
    case CacaoDatatype::Type::INT32:
        return py::format_descriptor<int32_t>::format();
    case CacaoDatatype::Type::UINT64:
        return py::format_descriptor<uint64_t>::format();
    case CacaoDatatype::Type::INT64:
        return py::format_descriptor<int64_t>::format();
    case CacaoDatatype::Type::FLOAT:
        return py::format_descriptor<float>::format();
    case CacaoDatatype::Type::DOUBLE:
        return py::format_descriptor<double>::format();
    // case CacaoDatatype::Type::COMPLEX_FLOAT: return py::format_descriptor<(std::complex<float>>::format();
    // case CacaoDatatype::Type::COMPLEX_DOUBLE: return py::format_descriptor<(std::complex<double>>::format();
    default:
        throw std::invalid_argument("Not implemented");
    }
}

CacaoDatatype PyFormatToCacaoDatatype(std::string pf)
{
    if (pf == py::format_descriptor<uint8_t>::format())
        return CacaoDatatype::Type::UINT8;
    if (pf == py::format_descriptor<int8_t>::format())
        return CacaoDatatype::Type::INT8;
    if (pf == py::format_descriptor<uint16_t>::format())
        return CacaoDatatype::Type::UINT16;
    if (pf == py::format_descriptor<int16_t>::format())
        return CacaoDatatype::Type::INT16;
    if (pf == py::format_descriptor<uint32_t>::format())
        return CacaoDatatype::Type::UINT32;
    if (pf == py::format_descriptor<int32_t>::format())
        return CacaoDatatype::Type::INT32;
    if (pf == py::format_descriptor<uint64_t>::format())
        return CacaoDatatype::Type::UINT64;
    if (pf == py::format_descriptor<int64_t>::format())
        return CacaoDatatype::Type::INT64;
    if (pf == py::format_descriptor<float>::format())
        return CacaoDatatype::Type::FLOAT;
    if (pf == py::format_descriptor<double>::format())
        return CacaoDatatype::Type::DOUBLE;
    // case CacaoDatatype::Type::COMPLEX_FLOAT: return py::format_descriptor<(std::complex<float>>::format();
    // case CacaoDatatype::Type::COMPLEX_DOUBLE: return py::format_descriptor<(std::complex<double>>::format();
    throw std::invalid_argument("Not implemented");
}

// Expose classes and methods to Python
PYBIND11_MODULE(pyImageStreamIO, m)
{
    m.doc() = "";

    auto cacaoDatatype = py::class_<CacaoDatatype>(m, "Datatype")
                          .def(py::init([](uint8_t atype) {
                              return std::unique_ptr<CacaoDatatype>(new CacaoDatatype(atype));
                          }))
                          .def_readonly("size", &CacaoDatatype::asize)
                          .def_readonly("type", &CacaoDatatype::atype);

    py::enum_<CacaoDatatype::Type>(cacaoDatatype, "Type")
        .value("UINT8", CacaoDatatype::Type::UINT8)
        .value("INT8", CacaoDatatype::Type::INT8)
        .value("UINT16", CacaoDatatype::Type::UINT16)
        .value("INT16", CacaoDatatype::Type::INT16)
        .value("UINT32", CacaoDatatype::Type::UINT32)
        .value("INT32", CacaoDatatype::Type::INT32)
        .value("UINT64", CacaoDatatype::Type::UINT64)
        .value("INT64", CacaoDatatype::Type::INT64)
        .value("FLOAT", CacaoDatatype::Type::FLOAT)
        .value("DOUBLE", CacaoDatatype::Type::DOUBLE)
        .value("COMPLEX_FLOAT", CacaoDatatype::Type::COMPLEX_FLOAT)
        .value("COMPLEX_DOUBLE", CacaoDatatype::Type::COMPLEX_DOUBLE)
        .value("EVENT_UI8_UI8_UI16_UI8", CacaoDatatype::Type::EVENT_UI8_UI8_UI16_UI8)
        .export_values();

    // IMAGE_KEYWORD interface
    py::class_<IMAGE_KEYWORD>(m, "Image_kw")
        .def(py::init([]() {
            return std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
        }))
        .def(py::init([](std::string name, int64_t numl, std::string comment) {
                 if (name.size() > 16)
                     throw std::invalid_argument("name too long");
                 if (comment.size() > 80)
                     throw std::invalid_argument("comment too long");
                 auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
                 std::copy(name.begin(), name.end(), kw->name);
                 kw->type = 'L';
                 kw->value.numl = numl;
                 std::copy(comment.begin(), comment.end(), kw->comment);
                 return kw;
             }),
             py::arg("name"),
             py::arg("numl"),
             py::arg("comment") = "")
        .def(py::init([](std::string name, double numf, std::string comment) {
                 if (name.size() > 16)
                     throw std::invalid_argument("name too long");
                 if (comment.size() > 80)
                     throw std::invalid_argument("comment too long");
                 auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
                 std::copy(name.begin(), name.end(), kw->name);
                 kw->type = 'D';
                 kw->value.numf = numf;
                 std::copy(comment.begin(), comment.end(), kw->comment);
                 return kw;
             }),
             py::arg("name"),
             py::arg("numf"),
             py::arg("comment") = "")
        .def(py::init([](std::string name, std::string valstr, std::string comment) {
                 if (name.size() > 16)
                     throw std::invalid_argument("name too long");
                 if (valstr.size() > 16)
                     throw std::invalid_argument("valstr too long");
                 if (comment.size() > 80)
                     throw std::invalid_argument("comment too long");
                 auto kw = std::unique_ptr<IMAGE_KEYWORD>(new IMAGE_KEYWORD());
                 std::copy(name.begin(), name.end(), kw->name);
                 kw->type = 'S';
                 std::copy(valstr.begin(), valstr.end(), kw->value.valstr);
                 std::copy(comment.begin(), comment.end(), kw->comment);
                 return kw;
             }),
             py::arg("name"),
             py::arg("valstr"),
             py::arg("comment") = "")
        .def_readonly("name", &IMAGE_KEYWORD::name)
        .def_readonly("type", &IMAGE_KEYWORD::type)
        .def("__repr__", [](const IMAGE_KEYWORD &kw) {
            std::ostringstream tmp_str;
            switch (kw.type)
            {
            case 'L':
                tmp_str << kw.value.numl;
                break;
            case 'D':
                tmp_str << kw.value.numf;
                break;
            case 'S':
                tmp_str << kw.value.valstr;
                break;
            default:
                tmp_str << "Unknown format";
                break;
            }
            return tmp_str.str();
        }) // TODO handle union
        .def_readonly("comment", &IMAGE_KEYWORD::comment);

    // IMAGE_METADATA interface
    py::class_<IMAGE_METADATA>(m, "Image_md")
        // .def(py::init([]() {
        //     return std::unique_ptr<IMAGE_METADATA>(new IMAGE_METADATA());
        // }))
        .def_readonly("name", &IMAGE_METADATA::name)
        .def_readonly("naxis", &IMAGE_METADATA::naxis)
        .def_property_readonly("size", [](const IMAGE_METADATA &md) {
            std::vector<uint32_t> dims(md.naxis);
            const uint32_t *ptr = md.size;
            for (auto &&dim : dims)
            {
                dim = *ptr;
                ++ptr;
            }
            return dims;
        })
        .def_readonly("nelement", &IMAGE_METADATA::nelement)
        .def_property_readonly("atype", [](const IMAGE_METADATA &md) {
            return CacaoDatatype(md.atype).atype;
        })
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
    py::class_<IMAGE>(m, "Image", py::buffer_protocol())
        .def(py::init([]() {
            return std::unique_ptr<IMAGE>(new IMAGE());
        }))
        .def_readonly("used", &IMAGE::used)
        .def_readonly("memsize", &IMAGE::memsize)
        .def_readonly("md", &IMAGE::md)
        .def_property_readonly("kw", [](const IMAGE &img) -> py::buffer_info {
            std::string format = py::format_descriptor<IMAGE_KEYWORD>::format();
            std::vector<ssize_t> shape = {img.md->NBkw};
            std::vector<ssize_t> strides = {img.md->NBkw};

            return py::buffer_info(
                img.kw,                 /* Pointer to buffer */
                sizeof(IMAGE_KEYWORD*), /* Size of one scalar */
                format,                 /* Python struct-style format descriptor */
                1,                      /* Number of dimensions */
                shape,                  /* Buffer dimensions */
                strides                 /* Strides (in bytes) for each index */
            );

        })
        .def_buffer([](const IMAGE &img) -> py::buffer_info {
            CacaoDatatype dt(img.md->atype);
            std::string format = CacaoDatatypeToPyFormat(dt);
            std::vector<ssize_t> shape(img.md->naxis);
            std::vector<ssize_t> strides(img.md->naxis);
            ssize_t stride = dt.asize;
            for (int8_t axis(img.md->naxis - 1); axis >= 0; --axis)
            {
                shape[axis] = img.md->size[axis];
                strides[axis] = stride;
                stride *= shape[axis];
            }
            return py::buffer_info(
                img.array.UI8, /* Pointer to buffer */
                dt.asize,      /* Size of one scalar */
                format,        /* Python struct-style format descriptor */
                img.md->naxis, /* Number of dimensions */
                shape,         /* Buffer dimensions */
                strides        /* Strides (in bytes) for each index */
            );

        })

        .def("write", [](IMAGE &img, py::buffer b) {
            /* Request a buffer descriptor from Python */
            py::buffer_info info = b.request();

            if (img.md->atype != PyFormatToCacaoDatatype(info.format).atype)
                throw std::invalid_argument("incompatible type");
            if (info.ndim != img.md->naxis)
                throw std::invalid_argument("incompatible number of axis");
            const uint32_t *size_ptr = img.md->size;
            for (auto &dim : info.shape)
            {
                if (*size_ptr != dim)
                    throw std::invalid_argument("incompatible shape");
                ++size_ptr;
            }

            CacaoDatatype dt(img.md->atype);
            uint8_t *buffer_ptr = (uint8_t *)info.ptr;
            uint64_t size = img.md->nelement * dt.asize;

            img.md->write = 1; // set this flag to 1 when writing data
            std::copy(buffer_ptr, buffer_ptr + size, img.array.UI8);

            std::vector<uint32_t> ushape(info.ndim);
            std::copy(info.shape.begin(), info.shape.end(), ushape.begin());
            ImageStreamIO_sempost(&img, -1);
            img.md->write = 0; // Done writing data
            img.md->cnt0++;
            img.md->cnt1++;

        },
             R"pbdoc(
          Write into memory image stream

          Parameters:
            buffer [in]:  buffer to put into memory image stream
          )pbdoc", py::arg("buffer"))

        .def("create", [](IMAGE &img, std::string name, py::array_t<uint32_t> b, uint8_t atype, uint8_t shared, uint16_t NBkw) {
            /* Request a buffer descriptor from Python */
            py::buffer_info info = b.request();

            // uint8_t atype = PyFormatToCacaoDatatype(info.format).atype;
            // std::vector<uint32_t> ushape(info.ndim);
            // std::copy(info.shape.begin(), info.shape.end(), ushape.begin());

            return ImageStreamIO_createIm(&img, name.c_str(), info.size, (uint32_t *)info.ptr, atype, shared, NBkw);
        },
             R"pbdoc(
            Create shared memory image stream

            Parameters:
                name   [in]:  the name of the shared memory file will be SHAREDMEMDIR/<name>_im.shm
                dims   [in]:  np.array of the image.
                atype  [in]:  data type code, pyImageStreamIO.Datatype
                shared [in]:  if true then a shared memory buffer is allocated.  If false, only local storage is used.
                NBkw   [in]:  the number of keywords to allocate.
            Return:
                ret    [out]: error code
            )pbdoc", py::arg("name"), py::arg("buffer"), py::arg("atype") = CacaoDatatype::Type::FLOAT, py::arg("shared") = 1, py::arg("NBkw") = 1)

        .def("link", [](IMAGE &img, std::string name) {
            return ImageStreamIO_read_sharedmem_image_toIMAGE(name.c_str(), &img);
        },
             R"pbdoc(
            Read / connect to existing shared memory image stream

            Parameters:
                name   [in]:  the name of the shared memory file to connect
            Return:
                ret    [out]: error code
            )pbdoc", py::arg("name"))

        .def("semwait", [](IMAGE &img, long index) {
            return ImageStreamIO_semwait(&img, index);
        },
             R"pbdoc(
            Read / connect to existing shared memory image stream

            Parameters:
                index  [in]:  semaphore index
            Return:
                ret    [out]: error code
            )pbdoc", py::arg("index"));
}
