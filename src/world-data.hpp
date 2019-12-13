#ifndef WORLD_DATA_HPP_
#define WORLD_DATA_HPP_

class WorldData {
private:
    GeometryData geometry_data_;
    PrimitiveData primitive_data_;

    VertexArray vertex_array_;
public:
    WorldData();
};

#endif
