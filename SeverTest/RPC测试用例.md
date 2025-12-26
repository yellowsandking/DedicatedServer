# ServerTestRPC 测试用例

## 类说明

`AServerTestRPC` 是一个演示 RPC（Remote Procedure Call）功能的类，包含三种类型的 RPC：

1. **Server RPC**: 客户端调用，在服务器执行
2. **Client RPC**: 服务器调用，在客户端执行
3. **NetMulticast RPC**: 服务器调用，在所有客户端执行

## RPC 类型说明

### Server RPC（服务器 RPC）

- **调用方向**: 客户端 → 服务器
- **执行位置**: 服务器端
- **特点**: 需要 `_Implementation` 和 `_Validate` 函数
- **用途**: 客户端向服务器发送请求或数据

**示例方法**:
- `ServerSendMessage()` - 客户端发送消息到服务器
- `ServerRequestCalculation()` - 客户端请求服务器计算
- `ServerRequestServerTime()` - 客户端请求服务器时间

### Client RPC（客户端 RPC）

- **调用方向**: 服务器 → 客户端
- **执行位置**: 客户端
- **特点**: 只需要 `_Implementation` 函数
- **用途**: 服务器向特定客户端发送数据

**示例方法**:
- `ClientReceiveMessage()` - 服务器发送消息到客户端
- `ClientReceiveCalculationResult()` - 服务器发送计算结果到客户端
- `ClientReceiveServerTime()` - 服务器发送时间到客户端

### NetMulticast RPC（多播 RPC）

- **调用方向**: 服务器 → 所有客户端
- **执行位置**: 所有客户端和服务器
- **特点**: 只需要 `_Implementation` 函数
- **用途**: 服务器向所有客户端广播消息

**示例方法**:
- `MulticastBroadcastMessage()` - 服务器广播消息到所有客户端
- `MulticastGameEvent()` - 服务器广播游戏事件

## 测试用例

### 测试用例 1: 基本 RPC 测试

#### 步骤：
1. 在场景中放置 `ServerTestRPC` Actor
2. 启动服务器（Listen Server 或 Dedicated Server）
3. 启动客户端并连接到服务器

#### 在服务器端测试：
```cpp
// 在服务器端调用
AServerTestRPC* RPCActor = GetWorld()->SpawnActor<AServerTestRPC>();
RPCActor->TestAllRPCs();
```

**预期结果**:
- 服务器和所有客户端都看到 Multicast 消息
- 所有客户端收到 Client RPC 消息

#### 在客户端测试：
```cpp
// 在客户端调用
AServerTestRPC* RPCActor = GetWorld()->SpawnActor<AServerTestRPC>();
RPCActor->ClientTestRPCs();
```

**预期结果**:
- 服务器收到 Server RPC 消息
- 服务器执行计算并返回结果到客户端
- 服务器返回时间到客户端

### 测试用例 2: Server RPC 测试

#### 测试 ServerSendMessage

**客户端调用**:
```cpp
AServerTestRPC* RPCActor = // 获取或创建RPCActor
RPCActor->ServerSendMessage(TEXT("Hello Server!"));
```

**预期结果**:
- 服务器日志显示: `[SERVER RPC] Received message from client: Hello Server!`
- 服务器屏幕显示黄色消息
- 服务器广播消息到所有客户端（通过 Multicast）

#### 测试 ServerRequestCalculation

**客户端调用**:
```cpp
RPCActor->ServerRequestCalculation(10.5f, 20.3f);
```

**预期结果**:
- 服务器计算: 10.5 + 20.3 = 30.8
- 客户端收到结果: `[CLIENT RPC] Calculation result: 30.80`

#### 测试 ServerRequestServerTime

**客户端调用**:
```cpp
RPCActor->ServerRequestServerTime();
```

**预期结果**:
- 客户端收到服务器时间
- 显示延迟信息

### 测试用例 3: Client RPC 测试

#### 测试 ClientReceiveMessage

**服务器端调用**:
```cpp
// 在服务器端
if (IsServer())
{
    RPCActor->ClientReceiveMessage(TEXT("Hello from server!"), TEXT("Server"));
}
```

**预期结果**:
- 客户端日志显示: `[CLIENT RPC] Received message from server: Hello from server!`
- 客户端屏幕显示青色消息

### 测试用例 4: NetMulticast RPC 测试

#### 测试 MulticastBroadcastMessage

**服务器端调用**:
```cpp
// 在服务器端
if (IsServer())
{
    RPCActor->MulticastBroadcastMessage(TEXT("Broadcast message!"), TEXT("Server"));
}
```

**预期结果**:
- 所有客户端和服务器都看到消息
- 服务器显示洋红色消息
- 客户端显示橙色消息

#### 测试 MulticastGameEvent

**服务器端调用**:
```cpp
// 在服务器端
if (IsServer())
{
    RPCActor->MulticastGameEvent(TEXT("PlayerJoined"), TEXT("Player1"));
}
```

**预期结果**:
- 所有客户端和服务器都看到事件
- 显示事件名称和数据

### 测试用例 5: 验证测试

#### 测试 RPC 验证函数

**客户端调用无效消息**:
```cpp
// 空消息（应该被拒绝）
RPCActor->ServerSendMessage(TEXT("")); // 验证失败

// 超长消息（应该被拒绝）
FString LongMessage = FString::ChrN(300, 'A'); // 300个字符
RPCActor->ServerSendMessage(LongMessage); // 验证失败
```

**预期结果**:
- 验证失败，RPC 不会执行
- 服务器不会收到消息

**客户端调用无效计算**:
```cpp
// 超大值（应该被拒绝）
RPCActor->ServerRequestCalculation(10000000.0f, 20000000.0f); // 验证失败
```

**预期结果**:
- 验证失败，RPC 不会执行

## 在 Blueprint 中使用

### 步骤：
1. 编译项目
2. 在 Content Browser 中找到 `ServerTestRPC`
3. 拖拽到场景中
4. 在 Blueprint 或 Level Blueprint 中调用方法

### Blueprint 节点：
- `Test All RPCs` - 测试所有 RPC（服务器端）
- `Client Test RPCs` - 测试客户端 RPC（客户端）
- `Server Send Message` - 发送消息到服务器
- `Server Request Calculation` - 请求计算
- `Server Request Server Time` - 请求服务器时间
- `Multicast Broadcast Message` - 广播消息
- `Multicast Game Event` - 广播游戏事件

## 日志输出说明

### 服务器端日志：
- `[SERVER RPC]` - Server RPC 执行
- `[MULTICAST RPC]` - Multicast RPC 执行（服务器端）

### 客户端日志：
- `[CLIENT RPC]` - Client RPC 执行
- `[MULTICAST RPC]` - Multicast RPC 执行（客户端）

### 屏幕消息颜色：
- **黄色**: Server RPC 消息
- **青色**: Client RPC 消息
- **洋红色**: Multicast RPC 消息（服务器）
- **橙色**: Multicast RPC 消息（客户端）

## 常见问题

### Q: RPC 没有执行？
A: 检查：
1. Actor 是否设置了 `bReplicates = true`
2. 是否在正确的端调用（Server RPC 从客户端调用，Client RPC 从服务器调用）
3. 网络连接是否正常

### Q: 验证失败？
A: 检查 `_Validate` 函数的返回值，确保参数符合验证条件

### Q: Client RPC 没有到达客户端？
A: 确保从服务器端调用，并且目标客户端已连接

### Q: Multicast 没有到达所有客户端？
A: 确保从服务器端调用，并且所有客户端都已连接

## 完整测试流程

1. **启动服务器**
   ```bash
   SeverTestServer.exe -log -server -game -port=7777
   ```

2. **启动客户端1**
   ```bash
   SeverTest.exe -game 127.0.0.1:7777
   ```

3. **启动客户端2**
   ```bash
   SeverTest.exe -game 127.0.0.1:7777
   ```

4. **在服务器端调用测试**
   - 调用 `TestAllRPCs()`
   - 观察所有客户端是否收到消息

5. **在客户端调用测试**
   - 在客户端1调用 `ClientTestRPCs()`
   - 观察服务器是否收到消息
   - 观察客户端1是否收到响应

6. **验证结果**
   - 检查日志输出
   - 检查屏幕消息
   - 确认所有 RPC 都正常工作

