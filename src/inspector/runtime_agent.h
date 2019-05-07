#ifndef SRC_INSPECTOR_RUNTIME_AGENT_H_
#define SRC_INSPECTOR_RUNTIME_AGENT_H_

#include "node/inspector/protocol/NodeRuntime.h"
#include "v8.h"

namespace node {
class Environment;

namespace inspector {
namespace protocol {

class RuntimeAgent : public NodeRuntime::Backend {
 public:
  explicit RuntimeAgent(Environment* env);

  void Wire(UberDispatcher* dispatcher);

  DispatchResponse enable() override;
  DispatchResponse disable() override;

  bool reportWaitingForDebuggerToDisconnect();

 private:
  std::shared_ptr<NodeRuntime::Frontend> frontend_;
  bool enabled_;
  Environment* env_;
};
}  // namespace protocol
}  // namespace inspector
}  // namespace node

#endif  // SRC_INSPECTOR_RUNTIME_AGENT_H_
