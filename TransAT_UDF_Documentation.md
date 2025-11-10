# TransAT UDF 使用文档

## 目录
1. [概述](#概述)
2. [UDF框架架构](#udf框架架构)
3. [基本UDF类型](#基本udf类型)
4. [高级UDF功能](#高级udf功能)
5. [数据访问接口](#数据访问接口)
6. [编译和执行](#编译和执行)
7. [实际应用示例](#实际应用示例)
8. [数学模型和公式](#数学模型和公式)
9. [最佳实践](#最佳实践)

---

## 概述

TransAT是一款通用的计算流体动力学(CFD)软件，类似于ANSYS Fluent，支持多相流模拟并提供用户定义函数(User-Defined Functions, UDF)功能。UDF允许用户通过编写C++代码来自定义和扩展TransAT的功能，实现特定的物理模型、边界条件、源项等。

### 主要特点
- **多语言支持**: 主要使用C++编程语言
- **多模块集成**: 可与求解器深度集成
- **实时交互**: 支持运行时干预和数据访问
- **并行计算**: 完全支持MPI并行环境
- **丰富接口**: 提供完整的数据结构和参数访问接口

---

## UDF框架架构

### 核心组件

#### 1. 接口头文件
```cpp
#include "cppinterface.h"  // 主要接口头文件
#include "EntryPointBase.decl.h"  // 入口点基类
```

#### 2. 工厂模式注册
TransAT使用工厂模式管理UDF：
```cpp
static EntryPointBase* create() { return new MyUDF; }
```

#### 3. 操作符重载
主要逻辑在`operator()`中实现：
```cpp
virtual void operator()() {
    // 用户代码
}
```

### 文件结构
每个UDF通常包含两个文件：
- `.cxx` - 实现文件
- `.h` - 头文件

---

## 基本UDF类型

### 1. 入口点UDF (Entry Point UDFs)

#### 调用时机
- **Begin Iteration**: 每次迭代前调用
- **End Iteration**: 每次迭代后调用  
- **Initialization**: 初始化阶段调用
- **Simulation Begin**: 模拟开始时调用
- **Simulation End**: 模拟结束时调用
- **Every Time Step**: 每个时间步调用（非定常）
- **Every Iteration**: 每次迭代调用（定常）
- **Output Frequency**: 按输出频率调用
- **Before Solver**: 求解器之前调用
- **Every Step Begin**: 每步开始时调用
- **Every Particle Substep**: 每个粒子子步骤调用

#### 编程模式
所有入口点UDF都继承自`EntryPoint_Base`基类：
```cpp
class MyUDF : public EntryPoint_Base {
public:
    MyUDF();
    virtual ~MyUDF();
    static EntryPoint_Base* create() { return new MyUDF; };
    virtual void operator()();
private:
    // 成员变量
};
```

#### 示例：最大温度监控UDF
```cpp
// getMaxTemp.cxx
#include <stdio.h>
#include <math.h>
#include "cppinterface.h"

getMaxTemp::getMaxTemp() {}

getMaxTemp::~getMaxTemp() {}

void getMaxTemp::operator()() {
    int nb, nijk, nblocks;
    double *T;
    double maxTemperature = 0.0;
    double globalMaxTemperature;
    
    // 获取处理器信息
    int globRank = MPI::COMM_WORLD.Get_rank();
    nblocks = get_integer("nblocks");
    
    // 遍历当前处理器的块
    for (nb = 1; nb <= nblocks; nb++) {
        nijk = get_integer(nb, "nijk");
        set_pointer(nb, &T, "temperature");
        
        // 寻找块内最大温度
        for (int ii = 0; ii < nijk; ii++) {
            maxTemperature = fmax(maxTemperature, T[ii]);
        }
    }
    
    // MPI通信获取全局最大值
    MPI::COMM_WORLD.Allreduce(&maxTemperature, &globalMaxTemperature, 
                              MPI::DOUBLE, MPI::MAX);
    
    // 主进程输出结果
    if (globRank == 0) {
        printf("Maximum temperature is %f\n", globalMaxTemperature);
    }
}
```

### 2. 源项UDF (Source Term UDFs)

#### 支持的方程类型
- **Alpha Rho**: 相体积分数方程源项
- **Concentration**: 浓度方程源项
- **Epsilon**: 湍流耗散率方程源项
- **Level-Set**: Level-Set方程源项
- **Mass Fraction**: 组分质量分数方程源项
- **Pressure**: 压力方程源项
- **TKE**: 湍流动能方程源项
- **Temperature**: 温度方程源项
- **U/V/W Velocity**: 速度分量方程源项

#### 编程模式
源项UDF同样继承自`EntryPoint_Base`，通过直接操作右端项(RHS)来实现：
```cpp
class ConcentrationSourceUDF : public EntryPoint_Base {
public:
    ConcentrationSourceUDF();
    virtual ~ConcentrationSourceUDF();
    static EntryPoint_Base* create() { return new ConcentrationSourceUDF; };
    virtual void operator()();
};
```

#### 示例：浓度源项UDF
```cpp
void ConcentrationSourceUDF::operator()() {
    int nblocks = get_integer("nblocks");
    int nijk;
    double *rhs, *x;
    double x_boundary = 0.1;  // 边界位置
    
    for (int nb = 0; nb < nblocks; nb++) {
        nijk = get_integer(nb, "nijk");
        set_pointer(nb, &rhs, "rhs");
        set_pointer(nb, &x, "cellcenterx");
        
        for (int ii = 0; ii < nijk; ii++) {
            // 在指定区域内添加源项
            if (x[ii] < x_boundary) {
                rhs[ii] += 23;  // 源项强度
            }
        }
    }
}
```

#### 示例：重力源项UDF
```cpp
void gravitySource::operator()() {
    int nblocks = get_integer("nblocks");
    double *rhs;
    double xgravity = get_double("xgravity");
    double rho = get_double("rhog");  // 重相密度
    
    for (int nb = 0; nb < nblocks; nb++) {
        int nijk = get_integer(nb, "nijk");
        set_pointer(nb, &rhs, "rhs");
        
        for (int ii = 0; ii < nijk; ii++) {
            // 添加重力源项
            rhs[ii] += 1000 * 9.81 * 0.2 * rho;
        }
    }
}
```

### 3. 物性UDF (Property UDFs)

#### 支持的物性类型
- **Equation of State**: 状态方程（密度、声速及其导数）
- **Heat Capacity**: 热容（比热容）
- **Latent Heat**: 潜热（常数或表格形式）
- **Saturation Curve**: 饱和曲线（常数或表格形式）

#### 编程模式
物性UDF继承自`UserDefinedPhaseProperties_Base`基类：
```cpp
class MyEOS : public UserDefinedPhaseProperties_Base {
public:
    MyEOS();
    virtual ~MyEOS();
    static UserDefinedPhaseProperties_Base* create() { return new MyEOS; };
    
    // 必须实现的虚函数
    virtual double Density(tmb::Properties_Manager* mgr, unsigned iphase);
    virtual double Soundspeed(tmb::Properties_Manager* mgr, unsigned iphase);
    virtual double DrhoDp(tmb::Properties_Manager* mgr, unsigned iphase);
    virtual double DrhoDp_t(tmb::Properties_Manager* mgr, unsigned iphase);
    virtual double DrhoDt_p(tmb::Properties_Manager* mgr, unsigned iphase);
private:
    double* _system_pressure;
    double _small;
    double _smdiff;
};
```

#### 示例：理想气体状态方程
```cpp
double MyEOS::Density(tmb::Properties_Manager* mgr, unsigned iphase) {
    double E = mgr->get<tmb::Energy>(iphase);
    double P = mgr->get<tmb::Pressure>(iphase) + *(_system_pressure);
    double Gamma = 1.4;
    
    P = std::max(_small, P);
    double dens = P / (E * (Gamma - 1.0) + _small);
    return dens;
}

double MyEOS::Soundspeed(tmb::Properties_Manager* mgr, unsigned iphase) {
    double T = mgr->get<tmb::Temperature>(iphase);
    double Gamma = 1.4;
    double R = tmb::Constants::GasConstant() * 1000.0 / 
               mgr->get<tmb::MolecularWeight>(iphase);
    
    return std::sqrt(Gamma * R * T);
}
```

#### 示例：常数热容
```cpp
double MyCp::Cp(tmb::Properties_Manager* mgr, unsigned iphase) {
    // 返回常数热容值 (J/kg·K)
    double cp = 1005;
    return cp;
}
```

### 4. 相间作用力UDF (Interfacial Force UDFs)

#### 曳力系数UDF
```cpp
class MyDrag : public DragTerm_Base {
public:
    MyDrag();
    virtual ~MyDrag();
    static DragTerm_Base* create() { return new MyDrag; };
    
    // 仅曳力情况
    virtual void operator()(double &ff, double &ffD, 
                           double vel_slip_mag, double Re, double Eo);
    
    // 包含升力和壁面润滑力的情况
    virtual void operator()(double &ff, double &ffDx, double &ffDy, double &ffDz,
                           double vel_slip_mag, double u_slip, double v_slip, double w_slip, 
                           double Re, double Eo);
private:
    double _small;
};
```

#### 示例：Schiller-Naumann曳力模型
```cpp
void MyDrag::operator()(double &ff, double &ffD, double vel_slip_mag, double Re, double Eo) {
    double Re_const = Re / (vel_slip_mag + _small);
    double constA = pow(Re_const, 0.687);
    double damp = 0.5 + 0.5 * std::tanh(Re / 100 - 10);
    double fun0 = 1 + 0.15 * pow(Re, 0.687);
    double fun1 = 0.0183 * Re;
    
    // 计算摩擦因子
    ff = (1 - damp) * fun0 + damp * fun1;
    
    // 计算导数（用于Newton-Raphson方法）
    double fun2 = Re_const / (200 * pow((std::cosh(Re/100 - 10)), 2) + _small);
    double fun3 = 0.10305 * constA * pow(std::abs(vel_slip_mag), -0.313);
    double fun4 = 0.0183 * Re_const;
    
    ffD = (vel_slip_mag / (std::abs(vel_slip_mag) + _small)) * 
          (fun2 * (fun1 - fun0) + damp * (fun4 - fun3) + fun3);
}
```

### 5. ParaView输出UDF

用于自定义变量的可视化输出：
```cpp
class CustomOutput : public Paraview_Base {
public:
    CustomOutput();
    virtual ~CustomOutput();
    static Paraview_Base* create() { return new CustomOutput; };
    virtual void operator()(double &var_out);
};

void CustomOutput::operator()(double &var_out) {
    // 计算绝对压力
    double gauge_pressure = properties_get("pressure");
    var_out = gauge_pressure + 101300;  // 转换为绝对压力
}
```

---

## 高级UDF功能

### 1. 质量传递系数UDF (Mass Transfer Coefficient UDFs)

#### 功能描述
质量传递系数UDF用于计算多孔介质或多相系统中的质量传递特性，支持常数和复杂的经验关联式。

#### 编程模式
```cpp
class MyMassTransferCoeff : public MasstransferCoeff_Base {
public:
    MyMassTransferCoeff();
    virtual ~MyMassTransferCoeff();
    virtual double operator()(int nb, int ii);  // nb: 块编号, ii: 单元格索引
private:
    // 枚举定义变量类型
    enum nb_arrays_enum {
        en_uvelocity, en_vvelocity, en_wvelocity,
        en_density, en_hembed, en_viscosity
    };
    
    // 数据结构
    typedef std::vector<double*> nb_pointers_t;
    typedef std::map<nb_arrays_enum, nb_pointers_t> nb_pointers_map;
    typedef std::map<nb_arrays_enum, std::string> nb_pointers_name_map;
    
    nb_pointers_map _nb_pointers;
    nb_pointers_name_map _nb_pointers_name;
    std::vector<double> _coefficients;
    double _lengthscale;
    double _small;
    int _nblocks;
    
    void set_nb_pointers();
    void read_coefficients();
    double getValue(nb_arrays_enum var, int nb, int ii);
};
```

#### 示例：线性拟合质量传递系数
```cpp
double MyMassTransferCoeff::operator()(int nb, int ii) {
    // 计算局部速度
    double loc_vel = sqrt(pow(getValue(en_uvelocity, nb, ii), 2) +
                         pow(getValue(en_vvelocity, nb, ii), 2) +
                         pow(getValue(en_wvelocity, nb, ii), 2));
    
    // 获取局部物性
    double loc_hembed = getValue(en_hembed, nb, ii);
    double loc_vel_s = loc_vel * loc_hembed;
    double loc_rho = getValue(en_density, nb, ii);
    double loc_visc = getValue(en_viscosity, nb, ii);
    
    // 计算雷诺数
    double Re = loc_vel_s * _lengthscale * loc_rho / (loc_visc + _small);
    
    // 线性拟合: Sh = coeff1 * Re^coeff2
    double mt_coefficient = (_coefficients.at(0) * pow(Re, _coefficients.at(1))) / _lengthscale;
    
    return mt_coefficient;
}
```

### 2. 孔隙阻力UDF (Porous Resistance UDFs)

孔隙阻力UDF用于模拟多孔介质中的流动阻力，可以基于达西定律或其他经验模型。

### 3. 内嵌风扇UDF

#### 功能描述
内嵌风扇UDF提供了射流风扇的集总参数表示，可以在模拟域内任意位置放置虚拟射流风扇。

#### 创建方法
```cpp
#include "Fan.h"

// 创建风扇对象
std::vector<double> centroid(3, 0), axis(3, 0);
centroid[0] = 1.5;  // X坐标
axis[0] = 1.0;     // X轴方向

Fan myFan("Fan1", 0.4, 1.5, centroid, axis, 10);
// 参数：名称、半径、长度、中心点、轴向、体积流量(m³/s)
```

#### 动态修改流量
```cpp
// 修改风扇体积流量
myFan.setVolumetricFlowRate(20);
```

### 2. 火源UDF

#### 功能描述
火源UDF将典型火灾的多个特征组合为单个UDF功能，包括热源和浓度源。

#### 创建方法
```cpp
#include "FireSource.h"

// 创建火源区域
RegionBase* fireBox = new BoxRegion("Fire1Region",
                                   0.0, 1.0,  // X范围
                                   0.0, 1.0,  // Y范围  
                                   0.0, 1.0); // Z范围

// 创建火源对象
FireSource myFire("Fire1", fireBox, 100000, 0.1);
// 参数：名称、区域、热源强度(W)、浓度源强度([C]/s)
```

---

## 数据访问接口

### 1. 参数访问

#### 整数参数
```cpp
int timestep = get_integer("timestep");
int nblocks = get_integer("nblocks");
int ni = get_integer(nb, "ni");  // 块特定参数
```

#### 双精度参数
```cpp
double currentTime = get_double("time");
 deltaTime = get_double("delt");
double xgravity = get_double("xgravity");
```

#### 逻辑参数
```cpp
bool solvingLevelset = get_logical("solve_levelset");
```

### 2. 变量数组访问

#### 设置指针访问变量
```cpp
double *u, *v, *w, *p, *T;

set_pointer(nb, &u, "uvelocity");
set_pointer(nb, &v, "vvelocity"); 
set_pointer(nb, &w, "wvelocity");
set_pointer(nb, &p, "pressure");
set_pointer(nb, &T, "temperature");
```

#### 多相变量访问
```cpp
double *alpha;
set_pointer(nb, &alpha, "alpha", phase_index);  // phase_index从0开始
```

### 3. 属性模块

#### 传统属性模块接口
```cpp
// 初始化属性模块
properties_initialize(nb, ii);

// 设置状态变量
properties_set_state("pressure", 150.0);
properties_set_state("temperature", 273.0);
properties_set_state("alpha", 0.003, 0);  // 第0相
properties_set_state("alpha", 0.997, 1);  // 第1相

// 获取属性值
double density_mixture = properties_get("density");
double density_phase = properties_get("density", 0);  // 特定相

// 应用更改
properties_set_back();
```

#### 新型属性管理器 (tmb::Properties_Manager)
```cpp
// 类型安全的属性访问
tmb::Properties_Manager* mgr;

// 获取各种物性
double E = mgr->get<tmb::Energy>(iphase);              // 内能
double T = mgr->get<tmb::Temperature>(iphase);         // 温度
double P = mgr->get<tmb::Pressure>(iphase);            // 压力
double rho = mgr->get<tmb::Density>(iphase);           // 密度
double mu = mgr->get<tmb::Viscosity>(iphase);          // 粘度
double cp = mgr->get<tmb::Cp>(iphase);                 // 热容
double lambda = mgr->get<tmb::Lambda>(iphase);         // 导热系数
double ss = mgr->get<tmb::Soundspeed>(iphase);         // 声速
double mw = mgr->get<tmb::MolecularWeight>(iphase);    // 分子量

// 获取导数
double drhodp_t = mgr->get<tmb::DrhoDp_t>(iphase);    // ∂ρ/∂p|T
double drhodt_p = mgr->get<tmb::DrhoDt_p>(iphase);    // ∂ρ/∂T|p
double drhodp = mgr->get<tmb::DrhoDp>(iphase);        // ∂ρ/∂p|s
```

#### 常量和安全参数
```cpp
// 获取系统常量
double R_gas = tmb::Constants::GasConstant();  // 气体常数
double small = tmb::Constants::Small();        // 小量防除零
double smdiff = tmb::Constants::Smdiff();      // 小量防比较误差

// 安全的数学运算
P = std::max(small, P);  // 避免除零
result = numerator / (denominator + small);
```

### 4. 网格和索引

#### 一维索引计算
```cpp
// 从3D坐标计算1D索引
int ii = i + j * ni + k * nij;
```

#### 3D到1D索引转换
```cpp
int i, j, k;
convert1DindexTo3D(nb, ii, i, j, k);
```

### 5. 模板类名和工厂模式

#### 模板类名约定
TransAT UDF使用`template_class_name`作为占位符，在实际使用时需要替换为具体的类名：

```cpp
// 模板文件中的写法
class template_class_name : public EntryPoint_Base {
public:
    template_class_name();
    virtual ~template_class_name();
    static EntryPoint_Base* create() { return new template_class_name; };
    virtual void operator()();
};

// 实际使用时的写法
class MyLineOutput : public EntryPoint_Base {
public:
    MyLineOutput();
    virtual ~MyLineOutput();
    static EntryPoint_Base* create() { return new MyLineOutput; };
    virtual void operator()();
};
```

#### 工厂模式注册机制
```cpp
// 每个UDF都必须实现静态create()方法
static EntryPoint_Base* create() { return new MyUDF; }

// 编译时会自动生成注册代码
// 在registerfunctors.cxx中注册所有UDF
```

#### 文件命名规范
- 头文件：`MyUDF.h`
- 实现文件：`MyUDF.cxx`
- 类名必须与文件名一致

---

## 编译和执行

### 1. 通过TransATUI编译

#### 步骤
1. 在User Defined Functions窗口点击"New"按钮
2. 定义UDF名称和类型
3. 选择模板或选择"None"
4. 点击"OK"创建UDF文件
5. 编辑UDF代码
6. 点击"Compile"编译UDF

#### 注意事项
- UDF名称必须是有效的C++文件名
- 不能包含特殊字符或以数字开头
- 类名必须与文件名相同

### 2. 命令行编译

#### 编译命令
```bash
tmb_link.py -u
```

#### 运行带UDF的模拟
```bash
tmb_run.py -n <processor_count> -u
```

### 3. 必需文件

编译过程会自动生成以下文件：
- `uifilelist.py`: 包含所有要编译文件的列表
- `registerfunctors.cxx`: 注册UDF指针到TransAT
- `registerfunctors.h`: registerfunctors.cxx的头文件

---

## 实际应用示例

### 1. 直线数据输出和监控

#### 基础用法
```cpp
#include "line_output.h"

lineUDF::lineUDF() {
    // 沿Z方向初始化直线，位于X=0.5, Y=0.05
    output = new line_output(LO_ZDIR, 0.5, 0.05, LO_BILINEAR);
    // 选择输出的变量
    output->add_variable("uvelocity");
    output->add_variable("temperature");
}

void lineUDF::operator()() {
    // 写入插值数据到文件
    output->write_file("output_data.dat");
}
```

#### 自定义变量输出
```cpp
void lineUDF::operator()() {
    // 计算雷诺数
    unsigned nblocks = get_integer("nblocks");
    double *u, *v, *w;
    double rho, mu, velocity;
    double L = 1.0;  // 特征长度
    
    for (int nb = 0; nb < nblocks; nb++) {
        unsigned nijk = get_integer(nb, "nijk");
        set_pointer(nb, &u, "uvelocity");
        set_pointer(nb, &v, "vvelocity");
        set_pointer(nb, &w, "wvelocity");
        
        for (unsigned ii = 0; ii < nijk; ii++) {
            properties_initialize(nb, ii);
            rho = properties_get("density");
            mu = properties_get("viscosity");
            velocity = sqrt(u[ii]*u[ii] + v[ii]*v[ii] + w[ii]*w[ii]);
            reynolds[ii_start + ii] = rho * velocity * L / mu;
        }
    }
    
    output->write_file("reynolds_data.dat");
}
```

### 2. 边界条件动态设置

#### 获取和设置边界值
```cpp
class BoundaryConditionsUDF : public EntryPoint_Base {
public:
    BoundaryConditionsUDF();
    virtual ~BoundaryConditionsUDF();
    static EntryPoint_Base* create() { return new BoundaryConditionsUDF; };
    virtual void operator()();
};

void BoundaryConditionsUDF::operator()() {
    int nblocks = get_integer("nblocks");
    
    for (int nb = 0; nb < nblocks; nb++) {
        // 获取边界信息
        int nFaces = get_integer(nb, "nfaces_north");
        
        for (int iface = 0; iface < nFaces; iface++) {
            // 获取当前边界值
            double currentTemp = get_boundary_value(nb, iface, "temperature", "north");
            
            // 动态设置新的边界值
            double newTemp = currentTemp * 1.1;  // 增加10%
            set_boundary_value(nb, iface, "temperature", newTemp, "north");
        }
    }
}
```

### 3. 壁面边界数据处理

#### 获取壁面信息和热通量计算
```cpp
int nblocks = get_integer("nblocks");

for (int nb = 0; nb < nblocks; nb++) {
    // 获取北边界面的数量
    int nWalls = getWallBoundaryNFaces(nb, "north");
    
    for (int iWall = 0; iWall < nWalls; iWall++) {
        // 获取壁面温度
        double Twall = getWallBoundaryValue(nb, iWall, "temperature", "north");
        
        // 获取相邻网格中心的温度
        int iiWallCell = getWallBoundaryCellIndex(nb, iWall, "north");
        properties_initialize(nb, iiWallCell);
        double Tcell = properties_get("temperature");
        
        // 计算热通量
        double wallDist = getWallBoundaryValue(nb, iWall, "walldistance", "north");
        double lambda = properties_get("lambda");
        double heatFluxFD = -(Tcell - Twall) / wallDist * lambda;
    }
}
```

### 4. 质量分数剖面输出

#### 定期输出质量分数分布
```cpp
class MassFractionProfiles : public EntryPoint_Base {
private:
    line_output *profile_output;
public:
    MassFractionProfiles();
    virtual ~MassFractionProfiles();
    static EntryPoint_Base* create() { return new MassFractionProfiles; };
    virtual void operator()();
};

MassFractionProfiles::MassFractionProfiles() {
    // 沿Y方向的剖面线
    profile_output = new line_output(LO_YDIR, 1.99, 0.0005, LO_BILINEAR);
    
    // 添加质量分数变量
    profile_output->add_variable("massfraction1");
    profile_output->add_variable("massfraction2");
    profile_output->add_variable("velocity");
}

void MassFractionProfiles::operator()() {
    // 写入剖面数据
    profile_output->write_file("mass_fraction_profiles.dat");
}
```

---

## 数学模型和公式

### 1. 基本控制方程

#### 连续性方程
$$
\frac{\partial \rho}{\partial t} + \frac{\partial (\rho u_j)}{\partial x_j} = 0
$$

#### 动量方程
$$
\frac{\partial (\rho u_i)}{\partial t} + \frac{\partial (\rho u_i u_j)}{\partial x_j} = -\frac{\partial p}{\partial x_i} + \frac{\partial \tau_{ij}}{\partial x_j} + \rho g_i + S_i
$$

其中应力张量：
$$
\tau_{ij} = \mu \left(\frac{\partial u_i}{\partial x_j} + \frac{\partial u_j}{\partial x_i}\right) - \frac{2}{3}\mu \frac{\partial u_k}{\partial x_k}\delta_{ij}
$$

#### 能量方程
$$
\frac{\partial (\rho h)}{\partial t} + \frac{\partial (\rho u_j h)}{\partial x_j} = \frac{\partial}{\partial x_j}\left(\frac{\lambda}{c_p}\frac{\partial h}{\partial x_j}\right) + S_h
$$

### 2. 多相流模型

#### Level-Set方法
界面演化方程：
$$
\frac{\partial \phi}{\partial t} + \mathbf{u} \cdot \nabla \phi = 0
$$

Heaviside函数：
$$
H(\phi) = \begin{cases}
0 & \phi < -\epsilon \\
\frac{1}{2}\left[1 + \frac{\phi}{\epsilon} + \frac{1}{\pi}\sin\left(\frac{\pi\phi}{\epsilon}\right)\right] & |\phi| \leq \epsilon \\
1 & \phi > \epsilon
\end{cases}
$$

#### 两相混合物性质
密度：
$$
\rho = \alpha_1 \rho_1 + \alpha_2 \rho_2
$$

粘度：
$$
\mu = \alpha_1 \mu_1 + \alpha_2 \mu_2
$$

### 3. 湍流模型

#### k-ε模型
湍流动能方程：
$$
\frac{\partial (\rho k)}{\partial t} + \frac{\partial (\rho u_j k)}{\partial x_j} = P_k - \rho \epsilon + \frac{\partial}{\partial x_j}\left[\left(\mu + \frac{\mu_t}{\sigma_k}\right)\frac{\partial k}{\partial x_j}\right]
$$

耗散率方程：
$$
\frac{\partial (\rho \epsilon)}{\partial t} + \frac{\partial (\rho u_j \epsilon)}{\partial x_j} = C_{\epsilon 1}\frac{\epsilon}{k}P_k - C_{\epsilon 2}\rho \epsilon + \frac{\partial}{\partial x_j}\left[\left(\mu + \frac{\mu_t}{\sigma_\epsilon}\right)\frac{\partial \epsilon}{\partial x_j}\right]
$$

湍流粘度：
$$
\mu_t = \rho C_\mu \frac{k^2}{\epsilon}
$$

### 4. 物质输运

#### 组分守恒方程
$$
\frac{\partial (\rho Y_k)}{\partial t} + \frac{\partial (\rho u_j Y_k)}{\partial x_j} = -\frac{\partial J_{kj}}{\partial x_j} + \omega_k
$$

扩散通量：
$$
J_{kj} = -\rho D_k \frac{\partial Y_k}{\partial x_j}
$$

### 5. 状态方程

#### 理想气体
$$
p = \rho R T
$$

#### 刚性气体方程
$$
p = (\gamma - 1)\rho e - \gamma p_\infty
$$

#### Tait方程（水）
$$
p = p_0 + B\left[\left(\frac{\rho}{\rho_0}\right)^N - 1\right]
$$

---

## 最佳实践

### 1. 编程模式和架构

#### 继承体系遵循
```cpp
// 正确的继承关系
class MyUDF : public EntryPoint_Base {           // 入口点UDF
class MyEOS : public UserDefinedPhaseProperties_Base {  // 物性UDF
class MyDrag : public DragTerm_Base {            // 曳力UDF
class MyOutput : public Paraview_Base {           // 输出UDF
```

#### 工厂模式实现
```cpp
// 每个UDF必须实现静态create方法
static BaseType* create() { return new MyUDF; }

// 构造函数中进行初始化
MyUDF::MyUDF() {
    // 获取系统常量
    _small = tmb::Constants::Small();
    _smdiff = tmb::Constants::Smdiff();
    
    // 设置指针到Fortran变量
    set_pointer(&_system_pressure, "system_pressure");
    
    // 初始化成员变量
    initializeData();
}
```

#### 模板类名替换
```cpp
// 开发时使用template_class_name
class template_class_name : public EntryPoint_Base {
    static EntryPoint_Base* create() { return new template_class_name; };
};

// 部署时替换为具体类名
class MyLineOutput : public EntryPoint_Base {
    static EntryPoint_Base* create() { return new MyLineOutput; };
};
```

### 2. 数据访问和安全

#### 类型安全的属性访问
```cpp
// 推荐：使用属性管理器
double density = mgr->get<tmb::Density>(iphase);
double temperature = mgr->get<tmb::Temperature>(iphase);

// 避免：字符串形式的属性访问
double density = properties_get("density");  // 不推荐
```

#### 数值安全措施
```cpp
// 防止除零错误
double safe_division(double numerator, double denominator) {
    return numerator / (denominator + _small);
}

// 防止无效比较
bool is_equal(double a, double b) {
    return fabs(a - b) < _smdiff;
}

// 限制变量范围
double clamp_value(double value, double min_val, double max_val) {
    return std::max(min_val, std::min(value, max_val));
}
```

#### 指针安全管理
```cpp
// 在构造函数中设置指针
MyUDF::MyUDF() {
    int nblocks = get_integer("nblocks");
    _velocity_data.resize(nblocks);
    
    for (int nb = 0; nb < nblocks; nb++) {
        set_pointer(nb, &_velocity_data[nb], "uvelocity");
    }
}

// 使用前检查指针有效性
void MyUDF::operator()() {
    if (!_velocity_data.empty()) {
        // 安全使用指针
        double u = _velocity_data[block_id][cell_id];
    }
}
```

### 3. 性能优化

#### 缓存和预计算
```cpp
class OptimizedUDF : public EntryPoint_Base {
private:
    int _nblocks;
    double _small;
    std::vector<int> _block_sizes;
    
public:
    OptimizedUDF() {
        // 缓存常用参数
        _nblocks = get_integer("nblocks");
        _small = tmb::Constants::Small();
        
        // 预计算块大小
        _block_sizes.reserve(_nblocks);
        for (int nb = 0; nb < _nblocks; nb++) {
            _block_sizes.push_back(get_integer(nb, "nijk"));
        }
    }
};
```

#### 循环优化
```cpp
// 推荐：使用局部变量减少函数调用
int nblocks = get_integer("nblocks");
for (int nb = 0; nb < nblocks; nb++) {
    int nijk = get_integer(nb, "nijk");
    double *u;
    set_pointer(nb, &u, "uvelocity");
    
    for (int ii = 0; ii < nijk; ii++) {
        // 处理逻辑
    }
}

// 缓存友好的访问模式
for (int k = 2; k < nk-3; k++) {
    for (int j = 2; j < nj-3; j++) {
        for (int i = 2; i < ni-3; i++) {
            int ii = i + j*ni + k*nij;
            // 访问连续内存
        }
    }
}
```

### 4. 并行处理

#### MPI最佳实践
```cpp
void parallel_operation() {
    int rank = MPI::COMM_WORLD.Get_rank();
    int size = MPI::COMM_WORLD.Get_size();
    
    // 本地计算
    double local_result = compute_local_quantity();
    
    // 全局聚合
    double global_result;
    MPI::COMM_WORLD.Allreduce(&local_result, &global_result, 
                              MPI::DOUBLE, MPI_SUM);
    
    // 只在主进程输出
    if (rank == 0) {
        printf("Global result: %f\n", global_result);
    }
}
```

#### 负载均衡考虑
```cpp
// 避免在单进程上执行过多工作
void balanced_workload() {
    int rank = MPI::COMM_WORLD.Get_rank();
    int size = MPI::COMM_WORLD.Get_size();
    
    // 将工作均匀分配给所有进程
    int total_work = get_total_work_items();
    int work_per_proc = total_work / size;
    int remainder = total_work % size;
    
    int start_idx = rank * work_per_proc + std::min(rank, remainder);
    int end_idx = start_idx + work_per_proc + (rank < remainder ? 1 : 0);
    
    for (int i = start_idx; i < end_idx; i++) {
        process_item(i);
    }
}
```

### 5. 错误处理和调试

#### 异常安全
```cpp
void safe_operation() {
    try {
        // 可能失败的操作
        risky_calculation();
    } catch (const std::exception& e) {
        if (get_integer("glob_rank") == 0) {
            printf("Error in UDF: %s\n", e.what());
        }
        // 设置默认值或跳过计算
        set_default_values();
    }
}
```

#### 边界检查和指针验证
```cpp
// 检查数组边界
if (ii >= 0 && ii < nijk) {
    // 安全访问
    value = array[ii];
} else {
    printf("Warning: Index out of bounds\n");
}

// 指针验证
double *ptr;
set_pointer(nb, &ptr, "variable_name");
if (ptr != nullptr) {
    // 安全使用指针
    ptr[ii] = newValue;
}
```

#### 调试输出控制
```cpp
class DebuggableUDF : public EntryPoint_Base {
private:
    bool _debug_mode;
    
public:
    DebuggableUDF() {
        // 通过参数控制调试模式
        _debug_mode = get_logical("enable_debug");
    }
    
    void debug_print(const std::string& message) {
        if (_debug_mode && get_integer("glob_rank") == 0) {
            printf("[DEBUG] %s\n", message.c_str());
        }
    }
};
```

### 6. 代码组织和维护

#### 模块化设计
```cpp
class MyUDF : public EntryPoint_Base {
private:
    // 私有成员变量
    std::vector<double> data_;
    
    // 私有辅助函数
    double calculateProperty(int nb, int ii);
    
public:
    // 公共接口
    MyUDF();
    virtual ~MyUDF();
    static EntryPoint_Base* create() { return new MyUDF; }
    virtual void operator()();
};
```

#### 配置文件读取
```cpp
class ConfigurableUDF : public EntryPoint_Base {
private:
    std::map<std::string, double> _config_params;
    
    void read_config_file() {
        std::ifstream file("udf_config.dat");
        std::string name;
        double value;
        
        while (file >> name >> value) {
            _config_params[name] = value;
        }
    }
    
    double get_param(const std::string& name, double default_val) {
        auto it = _config_params.find(name);
        return (it != _config_params.end()) ? it->second : default_val;
    }
};
```

#### 工具函数库
```cpp
// 将复杂计算分解为独立的纯函数
namespace udf_utils {
    double calculate_reynolds(double velocity, double length, 
                            double density, double viscosity) {
        return velocity * length * density / viscosity;
    }
    
    double schiller_naumann_drag(double Re) {
        if (Re < 0.1) return 0.0;
        
        double damp = 0.5 + 0.5 * tanh(Re / 100 - 10);
        double Cd_low = 1 + 0.15 * pow(Re, 0.687);
        double Cd_high = 0.0183 * Re;
        
        return (1 - damp) * Cd_low + damp * Cd_high;
    }
    
    bool is_in_region(double x, double y, double z, 
                     const std::vector<double>& bounds) {
        return (x >= bounds[0] && x <= bounds[1] &&
                y >= bounds[2] && y <= bounds[3] &&
                z >= bounds[4] && z <= bounds[5]);
    }
}
```

---

## 总结

TransAT的UDF功能为用户提供了强大而灵活的自定义能力，通过合理使用各种UDF类型和接口，可以实现复杂的物理模型和工程应用。本文档涵盖了从基础概念到高级应用的各个方面，为用户提供了完整的UDF开发指南。

### 关键要点回顾

1. **架构理解**: 掌握TransAT的多块结构和数据访问模式
2. **接口熟悉**: 熟练使用各种参数和变量访问接口
3. **性能考虑**: 注意并行效率和内存访问模式
4. **调试技能**: 具备有效的问题诊断和解决能力
5. **最佳实践**: 遵循编码规范和优化原则

通过不断实践和学习，用户可以充分利用TransAT UDF的强大功能，解决复杂的流体力学问题。