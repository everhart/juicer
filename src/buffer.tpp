namespace juicer {

template <class T> Buffer<T>::Buffer(
    const std::vector<GLfloat>& data, GLsizei stride, GLenum type
) : data_(data), stride_(stride), type_(type) 
{
}

template <class T> T * Buffer<T>::data() {
    return data_.ptr();
}

template <class T> const T * Buffer<T>::data() const noexcept {
    return data_.ptr();
}

template <class T> GLsizei Buffer<T>::size() const noexcept {
    return data_.size();
}

template <class T> GLsizei Buffer<T>::stride() const noexcept {
    return stride_;
}
template <class T> GLenum Buffer<T>::type() const noexcept {
    return type_;
}

}
