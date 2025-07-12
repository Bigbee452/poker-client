#pragma once

class index_buffer {
    public:
        index_buffer();
        ~index_buffer();
        void set_data(unsigned int* data, unsigned int count);
        void bind();
        void unbind();
    private:
        unsigned int buffer_id;
};