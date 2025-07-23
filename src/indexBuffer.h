#pragma once

#include <vector>
class index_buffer {
    public:
        index_buffer();
        ~index_buffer();
        void set_data(unsigned int* data, unsigned int in_count);
        void set_data(std::vector<unsigned int> indices);
        void bind();
        void unbind();
        int count = 0;
    private:
        unsigned int buffer_id;
};