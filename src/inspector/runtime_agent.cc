#include "runtime_agent.h"

#include "env-inl.h"
#include "inspector_agent.h"

namespace node {
namespace inspector {
namespace protocol {

RuntimeAgent::RuntimeAgent(Environment* env)
  : enabled_(false)
  , env_(env) {}

void RuntimeAgent::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new NodeRuntime::Frontend(dispatcher->channel()));
  NodeRuntime::Dispatcher::wire(dispatcher, this);
}

DispatchResponse RuntimeAgent::enable() {
  if (!env_->owns_process_state()) {
    return DispatchResponse::Error(
        "NodeRuntime domain can only be used through main thread sessions");
  }
  enabled_ = true;
  return DispatchResponse::OK();
}

DispatchResponse RuntimeAgent::disable() {
  if (!env_->owns_process_state()) {
    return DispatchResponse::Error(
        "NodeRuntime domain can only be used through main thread sessions");
  }
  enabled_ = false;
  return DispatchResponse::OK();
}

bool RuntimeAgent::reportWaitingForDebuggerToDisconnect() {
  if (enabled_) frontend_->waitingForDebuggerToDisconnect();
  return enabled_;
}
}  // namespace protocol
}  // namespace inspector
}  // namespace node
