namespace juicer {

template <class T> void VertexArray::push(
    VertexAttribute<T>& vertex_attribute
) 
{
    Buffer<T> buffer = vertex_attribute.buffer();
    buffer.bind();
    glEnableVertexAttribArray(size_);
    glVertexAttribPointer(
        size_++, 
        buffer.stride(), 
        buffer.type(), 
        GL_FALSE, 
        0, 
        buffer.data() + buffer.offset()
    );
    buffer.unbind();
}

}
