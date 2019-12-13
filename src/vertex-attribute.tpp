namespace juicer {

template <class T> VertexAttribute<T>::VertexAttribute(
    Buffer<T>& buffer, GLsizei offset, GLsizei stride
) : buffer_(buffer), 
    offset_(offset), 
    stride_(stride)
{   
}

template <class T> Buffer<T>& VertexAttribute<T>::buffer() {
    return buffer_;
}

template <class T> GLsizei VertexAttribute<T>::offset() {
    return offset_;
}

template <class T> GLsizei VertexAttribute<T>::stride() {
    return stride_;
}

}
