#include "primitive-data.hpp"

namespace juicer {

PrimitiveData::PrimitiveData() 
{
    vertex_pool_ = VertexPool(8192);
}

PrimitiveData& PrimitiveData::instance() {
    if (instance_ == nullptr) {
        instance_ = new PrimitiveData();
    }

    return instance_;
}

}
