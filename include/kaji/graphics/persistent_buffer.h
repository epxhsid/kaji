#pragma once

#include "kaji/shader.h"
#include <cstddef>
#include <optional>
namespace graphics {
class PersistentBuffer {
public:
    static constexpr std::size_t slot_count = 3;

    struct Region {
        std::byte* data;
        GLintptr offset;
        GLsizeiptr size;
        std::size_t slot;
    };

    explicit PersistentBuffer(GLsizeiptr slot_size);
    ~PersistentBuffer();

    PersistentBuffer(const PersistentBuffer&) = delete;
    PersistentBuffer& operator=(const PersistentBuffer&) = delete;

    PersistentBuffer(PersistentBuffer&&) = delete;
    PersistentBuffer& operator=(PersistentBuffer&&) = delete;

    [[nodiscard]]
    std::optional<Region> try_acquire();

    void mark_submitted(std::size_t slot);
    void cancel(std::size_t slot);

    [[nodiscard]]
    GLuint id() const noexcept;

    [[nodiscard]]
    GLsizeiptr slot_size() const noexcept;

private:
    GLuint buffer_ = 0;
    std::byte* mapped_ = nullptr;
    GLsizeiptr slot_size_ = 0;
    GLsync fences_[slot_count]{};
    std::size_t next_slot_ = 0;
    std::size_t active_slot_ = 0;
    bool active_ = false;
};

}
