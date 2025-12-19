# 在编辑器 PIE 模式下连接 Dedicated Server

## 方法 1：通过编辑器设置（推荐）

### 步骤：

1. **打开编辑器设置**
   - 点击 `Edit` → `Editor Preferences`（或 `Edit` → `Preferences`）
   - 在左侧找到 `Play` → `Multiplayer Options`

2. **配置连接设置**
   - **Play Number Of Clients**: 设置为 `1`（如果只启动一个客户端）
   - **Run Dedicated Server**: **取消勾选**（如果要连接外部服务器）
   - **Net Mode**: 选择 `Play As Client`
   - **Server IP Address**: 输入服务器 IP 地址（例如：`127.0.0.1` 或 `192.168.1.100`）
   - **Server Port**: 输入服务器端口（例如：`7777`）

3. **启动连接**
   - 点击 `Play` 按钮
   - 编辑器会以客户端模式启动，并自动连接到指定的服务器

### 注意事项：
- 确保 Dedicated Server 已经启动并正在运行
- 确保服务器 IP 和端口正确
- 确保防火墙允许连接

## 方法 2：通过命令行参数（在编辑器外部）

如果你在编辑器外部运行客户端，可以使用命令行参数：

```bash
# 连接到本地服务器
SeverTest.exe -game 127.0.0.1:7777

# 连接到远程服务器
SeverTest.exe -game 192.168.1.100:7777
```

## 方法 3：通过代码自动连接

可以在 `ServerTestPlayerController` 的 `BeginPlay` 中添加自动连接逻辑（仅用于开发测试）。

## 验证连接

连接成功后，你应该看到：
- 客户端窗口显示 `[CLIENT]` 绿色日志
- 服务器控制台显示玩家连接信息
- `ServerTestActor` 在客户端显示绿色日志

## 常见问题

### Q: 连接失败？
A: 检查：
1. 服务器是否正在运行
2. IP 地址和端口是否正确
3. 防火墙是否阻止连接
4. 服务器日志中是否有错误信息

### Q: 如何查看当前连接的服务器？
A: 在客户端窗口中，查看日志输出，会显示网络模式信息。

### Q: 可以同时连接多个客户端吗？
A: 可以，设置 `Play Number Of Clients` 为多个，每个客户端都会连接到同一个服务器。

