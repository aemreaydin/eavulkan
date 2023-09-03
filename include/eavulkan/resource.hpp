#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

class Device;
template <typename VulkanObject>
class DeviceResource {
  Resource<VulkanObject> _resource{};
  const class Device *_pDevice{nullptr};

 public:
  explicit DeviceResource(const Device &device) : _pDevice(&device) {}
  DeviceResource(const Device &device, VulkanObject resource) : _pDevice(&device), _resource(resource) {}

  virtual ~DeviceResource() = default;
  DeviceResource(const DeviceResource &) = default;
  auto operator=(const DeviceResource &) -> DeviceResource & = default;
  DeviceResource(DeviceResource &&) noexcept = default;
  auto operator=(DeviceResource &&) noexcept -> DeviceResource & = default;

  [[nodiscard]] auto GetDevice() const -> const Device & { return *_pDevice; }

  [[nodiscard]] auto GetHandle() -> VulkanObject & { return _resource; }

  [[nodiscard]] auto GetHandle() const -> const VulkanObject & { return _resource; }

  virtual auto Cleanup() -> void = 0;
};

}  // namespace Rendering
