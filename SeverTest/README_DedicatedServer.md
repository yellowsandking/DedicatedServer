# Dedicated Server Demo 使用说明

这个demo展示了如何在Unreal Engine中使用Dedicated Server（专用服务器）。

## 项目结构

### 核心类文件

1. **ServerTestGameMode** (`ServerTestGameMode.h/cpp`)
   - 游戏模式类，管理服务器端的游戏逻辑
   - 跟踪玩家连接和断开
   - 在服务器端输出日志信息

2. **ServerTestPlayerController** (`ServerTestPlayerController.h/cpp`)
   - 玩家控制器，处理客户端-服务器通信
   - 实现RPC（远程过程调用）示例
   - 处理玩家输入并发送到服务器

3. **ServerTestActor** (`ServerTestActor.h/cpp`)
   - 演示Actor，展示服务器-客户端同步
   - 在服务器端控制移动和旋转
   - 客户端自动接收同步的位置和旋转

4. **ServerTestGameState** (`ServerTestGameState.h/cpp`)
   - 游戏状态类，同步服务器状态到所有客户端
   - 跟踪服务器运行时间

5. **SeverTestServer.Target.cs**
   - 专用服务器构建目标
   - 用于构建无渲染的服务器版本

## 构建和运行

### 1. 生成项目文件

在项目根目录运行：
```
右键点击 SeverTest.uproject -> Generate Visual Studio project files
```
或者使用命令行：
```
UnrealVersionSelector.exe -projectfiles -project="G:\UESample\SeverTest\SeverTest.uproject" -game -rocket -progress
```

### 2. 编译项目

在Visual Studio中：
- 选择 `Development Editor` 配置用于编辑器测试
- 选择 `Development Server` 配置用于构建专用服务器

或者使用命令行：
```bash
# 编译编辑器版本
UnrealBuildTool.exe SeverTestEditor Win64 Development "G:\UESample\SeverTest\SeverTest.uproject" -WaitMutex -FromMsBuild

# 编译专用服务器
UnrealBuildTool.exe SeverTestServer Win64 Development "G:\UESample\SeverTest\SeverTest.uproject" -WaitMutex -FromMsBuild
```

### 3. 在编辑器中测试（PIE模式）

1. 打开项目
2. 在项目设置中设置默认GameMode为 `ServerTestGameMode`
3. 点击 `Play` -> `Number of Players` 设置为2或更多
4. 选择 `Net Mode` 为 `Play As Client` 或 `Play As Listen Server`

### 4. 运行专用服务器

#### 方法1：使用命令行启动服务器

```bash
# 启动专用服务器（无渲染）
SeverTestServer.exe -log -server -game -port=7777

# 或者指定地图
SeverTestServer.exe -log -server -game -port=7777 /Game/Maps/YourMap
```

#### 方法2：使用编辑器启动服务器

1. 在编辑器中，打开 `Edit` -> `Editor Preferences` -> `Play`
2. 设置 `Net Mode` 为 `Play As Listen Server` 或 `Dedicated Server`
3. 点击 `Play`

### 5. 客户端连接

#### 方法1：使用命令行连接

```bash
# 连接到本地服务器
SeverTest.exe -game 127.0.0.1:7777

# 连接到远程服务器
SeverTest.exe -game <ServerIP>:7777
```

#### 方法2：在编辑器中连接

1. 启动一个服务器实例（PIE模式，设置为Listen Server）
2. 启动另一个客户端实例（PIE模式，设置为Client）
3. 客户端会自动连接到服务器

## 功能演示

### 1. 服务器-客户端同步

- `ServerTestActor` 在服务器端执行移动和旋转逻辑
- 客户端自动接收同步的位置和旋转
- 所有客户端看到相同的Actor位置

### 2. RPC通信

- **客户端到服务器**：`ServerSendMessage` - 客户端发送消息到服务器
- **服务器到客户端**：`ClientReceiveMessage` - 服务器广播消息到所有客户端
- **时间同步**：`ServerRequestServerTime` / `ClientReceiveServerTime` - 同步服务器时间

### 3. 玩家连接管理

- `PostLogin` - 玩家加入时调用
- `Logout` - 玩家离开时调用
- `GetPlayerCount` - 获取当前玩家数量

### 4. 游戏状态同步

- `ServerTestGameState` 同步服务器运行时间到所有客户端
- 所有客户端可以访问相同的游戏状态

## 配置说明

### 在项目设置中配置

1. 打开 `Edit` -> `Project Settings`
2. 在 `Game` -> `Default Modes` 中设置：
   - **Default GameMode**: `ServerTestGameMode`
   - **Default Pawn Class**: （可选，根据需要设置）
   - **Player Controller Class**: `ServerTestPlayerController`
   - **Game State Class**: `ServerTestGameState`

### 在DefaultEngine.ini中配置

在 `Config/DefaultEngine.ini` 中添加：

```ini
[/Script/EngineSettings.GameMapsSettings]
ServerDefaultMap=/Game/Maps/YourMap
GameDefaultMap=/Game/Maps/YourMap

[/Script/OnlineSubsystemUtils.IpNetDriver]
NetDriverClassName=/Script/OnlineSubsystemUtils.IpNetDriver
```

## 测试步骤

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

4. **观察行为**
   - 服务器控制台显示玩家连接信息
   - 客户端可以看到同步的Actor移动
   - 按空格键（Jump）会发送消息到服务器
   - 服务器会广播消息到所有客户端

## 网络架构说明

### 服务器端（Authority）
- 拥有游戏世界的权威状态
- 执行所有游戏逻辑
- 同步状态到客户端

### 客户端
- 接收服务器同步的数据
- 发送输入和请求到服务器
- 显示游戏画面

### RPC类型
- **Server RPC**: 客户端调用，在服务器执行
- **Client RPC**: 服务器调用，在客户端执行
- **NetMulticast RPC**: 服务器调用，在所有客户端执行

## 常见问题

### Q: 服务器启动后立即退出？
A: 检查是否有错误日志，确保地图路径正确。

### Q: 客户端无法连接？
A: 检查防火墙设置，确保端口7777开放。

### Q: Actor不同步？
A: 确保Actor设置了 `bReplicates = true` 和 `SetReplicatingMovement(true)`。

### Q: RPC不工作？
A: 确保函数标记了正确的RPC宏（`Server`, `Client`, `NetMulticast`），并且有对应的 `_Implementation` 和 `_Validate` 函数。

## 下一步

- 添加更多游戏逻辑
- 实现玩家角色同步
- 添加网络延迟补偿
- 实现服务器端验证
- 添加反作弊机制

## 参考资料

- [Unreal Engine Networking Documentation](https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/)
- [Dedicated Server Guide](https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/DedicatedServers/)

