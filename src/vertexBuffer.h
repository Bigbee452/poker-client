#pragma once

class vertex_buffer {
    public:
        vertex_buffer();
        ~vertex_buffer();
        void set_data(float* data, unsigned int count);
        void bind();
        void unbind();
    private:
        unsigned int buffer_id;
        unsigned int vertex_array_id;
};