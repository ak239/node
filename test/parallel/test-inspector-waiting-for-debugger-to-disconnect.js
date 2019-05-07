// Flags: --expose-internals
'use strict';
const common = require('../common');

common.skipIfInspectorDisabled();

const assert = require('assert');
const { NodeInstance } = require('../common/inspector-helper.js');

async function runTest() {
  const child = new NodeInstance(['--inspect-brk=0', '-e', 'process.exit(55)']);
  const session = await child.connectInspectorSession();
  await session.send([
    { method: 'Runtime.enable' },
    { method: 'NodeRuntime.enable' },
    { method: 'Runtime.runIfWaitingForDebugger' }]);
  await session.waitForNotification((notification) => {
    return notification.method === 'NodeRuntime.waitingForDebuggerToDisconnect';
  });
  const receivedExecutionContextDestroyed = session.unprocessedNotifications().some((notification) => {
    return notification.method === 'Runtime.executionContextDestroyed' &&
      notification.params.executionContextId === 1;
  });
  if (receivedExecutionContextDestroyed) {
    assert.fail(`When NodeRuntime enabled, Runtime.executionContextDestroyed should not be sent`);
  }
  await session.disconnect();
  assert.strictEqual((await child.expectShutdown()).exitCode, 55);
}

runTest();
