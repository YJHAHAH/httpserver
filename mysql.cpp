create table employee(
    id int comment '编号',
    workno varchar(10) comment '工号',
    name varchar(10) comment '姓名',
    gender char(1) comment '性别',
    age tinyint unsigned comment '年龄',
    idcard char(18) comment '身份证号',
    entrydate date comment '入职时间'
    username varchar(30) 
) comment '员工表';
alter table employee add address varchar(20) comment '地址';//增加
alter table employee modify address varchar(30);//修改数据类型
alter table employee change address addresses varchar(25) comment '地址';//修改字段名
alter table employee rename to employ;//修改表名；
drop table if exists emp;//删除
truncate table emp;
//desc employee
//use database;
//show tables;
//show databases;
//drop add change modify
insert into employee(id,workno,name,gender,age,entrydate) values(1,1,'hashd','男',26,'2020-01-01');//插入数据
insert into employee values(3,3,'haal','女',23,'511112199701255356','2001-02-27','hasl'),
(4,4,'haap','女',25,'511112199701255358','2001-02-27','haslp');//批量插入
insert into employee values(5,5,'haa5l','女',27,'511112199701255358','2001-03-27','has2l','cq'),
(6,6,'ha5ap','女',28,'511112199701255658','2001-05-27','hasl2p','pk');
insert into employee values(7,7,'haa7l','男',27,'511112199701255758','2001-03-27','has7l','cq'),
(8,8,'ha8ap','男',28,'511112199781255658','2001-05-27','hasl8p','pk');
select * from employee;
//修改
update employee set name = 'hashii' where id = 1;
update employee set name='hahsi',gender='女' where id=1;
update employee set entrydate ='2008-01-01';
update employee set name='hahsi',gender='男' where id=1;
//删除
delete from employee where gender='男';
delete from employee;
//DQL ·基本查询语法
//查询指定字段
select name,workno,age from employee;
select id,workno,name,gender,idcard from employee;
select * from employee;//查询所有字段
//起别名
select id as '编号' from employee;//as可去掉 别名
select id ID from employee;
//返回 去重 distinct???
select distinct gender from employee;
//条件查询
select * from employee where age=25;
select * from employee where age<25;
select * from employee where age<=25;
select * from employee where addresses is null;
select * from employee where addresses is not null;
select * from employee where age != 25;
select * from employee where age <> 25;//不等于
select * from employee where age <= 25 && age >= 20;
select * from employee where age <= 25 and age >= 20;
select * from employee where age between 20 and 25;//之间 最小 到 最大 之间
select * from employee where gender='女' and age < 25;//与
select * from employee where age = 23 or age =25 or age =27;//或
select * from employee where age in (23,25,27);
select * from employee where name like '___';//模糊查询 名字字数
select * from employee where idcard like '%8';//末尾为8
//聚合查询
select count(*) from employee;//数据总数 null 不计算
select avg(age) from employee;
select max(age) from employee;
select min(age) from employee;
select sum(age) from employee where gender='女';
//分组查询
select gender,count(*) from employee group by gender;//根据性别分组 统计数量
select gender,avg(age) from employee group by gender;//根据性别分组 统计平均年龄
select addresses,count(*) from employee where age<35 group by addresses having count(*)<=2;//按地址分组 年龄大于35 数量小于3
//排序
select * from employee order by age;//默认 asc
select * from employee order by age desc;
select * from employee order by age desc, entrydate asc;
//分页
select * from employee limit 0,3;//开始页码  每页条数
select * from employee limit 3,3;//第二页开始 （页码-1）*每页数目
//DCL 访问权限
//查询用户
use mysql;
select * from user;
//创建用户
create user 'itcast'@'localhost' identified by 'YJHhaha123..';
create user 'yjh'@'%' identified by 'YJHhaha123..';//任何地方均可登陆
//修改密码 ROOT yujinghuiK12345..
alter user 'itcast'@'localhost' identified with mysql_native_password by 'YJHhahaha123..';
//删除用户
drop user 'itcast'@'localhost';
//权限控制
//查询权限
show grants for 'yjh'@'%';
//授予权限 针对某一数据库的权限
grant all on mytest.* to 'yjh'@'%';
//撤销权限 *.*所有数据库所有表 超级管理员
revoke all on mytest.* from 'yjh'@'%';
//字符串函数
select concat('hello','mysql');//拼接
select lower('HELLO');//小写
select upper('mysal');//大写
select lpad('01',5,'-');//左填充
select rpad('01',5,'+');//右边
select trim(' hello  mysql  ');//去掉首尾空格
select substring('hello world',1,5);//截取
update employee set workno = lpad(workno,5,'0');//修改 填充 工号
//数值函数
select ceil(1.1);//向上取整
select floor(1.9);//向下
select mod(7,4);
select rand();
select round(2.345,2);//四舍五入 两位小数
//随机数 六位
select lpad(round(rand()*1000000,0),6,'0');//填充0 四舍五入 随机数
//日期函数
select curdate();
select curtime();
select now();
select year(now());//获取年份
select month(now());
select day(now());
//时间间隔
select date_add(now(),interval 70 day);
select date_add(now(),interval 70 month);
//datediff 相差天数
select datediff('2021-12-01','2022-5-17');//可负数
select name,datediff(curdate(),entrydate) as entrydays from employee order by entrydays desc; 
//流程控制函数
select if(true,'ok','error');
select if(false,'ok','error');
select ifnull('','default');
select ifnull('hahh','default');//不是null 就返回前一个
select ifnull(null,'default');

select 
 name,
 (case addresses when 'pk' then 'first' when 'cq' then 'second' else 'else' end) as 'workaddress'//别名
from employee;

select 
id,
name,
(case when math>=85 then '优秀' when math >=60 then '及格' else '不及格' end) 'math'
(case when chinese>=85 then '优秀' when math >=60 then '及格' else '不及格' end) 'chinese'
(case when english>=85 then '优秀' when math >=60 then '及格' else '不及格' end) 'english'
from score;


//约束
create table user(
    id int primary key auto_increment comment '主键',
    name varchar(10) not null unique comment '姓名',
    age int check(age>0 and age <=120) comment '年龄',
    status char(1) default '1' comment '状态',  //默认值
    gender char(1) comment '性别'
) comment '用户表';

insert into user(name,age,status,gender) values ('tom',19,'1','男'),('lily',20,'1','女');
insert into user(name,age,status,gender) values ('tom',121,'1','男'),('lily',20,'1','女');//无效数据

//外键约束
create table emp(
    id int primary key auto_increment, //自增 主键 配对
    name varchar(15) not null,
    age int,
    job varchar(20),
    salary int,
    entrydate date,
    magagerid int comment '领导ID',
    dept_id int comment '部门ID'
) comment '员工表';
insert into emp (id,name,age,job,salary,entrydate,magagerid,dept_id) values (1,'hah',66,'boss',20000,'2000-01-02',null,5),
(2,'hah5',60,'worker',2000,'2000-01-03',1,5),(3,'ha8h',61,'guard',2000,'2000-01-03',2,4);
//添加外键约束
alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);
//多对多 中间表
create table student(
    id int auto_increment primary key comment '主键ID',
    name varchar(10) comment '姓名',
    no varchar(10) comment '学号'
) comment '学生表';
insert into student values (null, '黛绮丝', '2000100101'),
(null, '谢逊', '2000100102'),(null, '殷天正', '2000100103'),
(null, '韦一笑', '2000100104');

create table course(
    id int auto_increment primary key comment '主键ID',
    name varchar(10) comment '课程名称'
) comment '课程表';
insert into course values (null, 'Java'), (null, 'PHP'), (null , 'MySQL') , (null, 'Hadoop');
//中间表
create table student_course(
    id int auto_increment comment '主键' primary key,
    studentid int not null comment '学生ID',
    courseid  int not null comment '课程ID',
    constraint fk_courseid foreign key (courseid) references course (id),//外键约束
    constraint fk_studentid foreign key (studentid) references student (id)
)comment '学生课程中间表';
insert into student_course values (null,1,1),(null,1,2),(null,1,3),(null,2,2),(null,2,3),(null,3,4);

//一对一
create table tb_user(
    id int auto_increment primary key comment '主键ID',
    name varchar(10) comment '姓名',
    age int comment '年龄',
    gender char(1) comment '1: 男 , 2: 女',
    phone char(11) comment '手机号'
) comment '用户基本信息表';
create table tb_user_edu(
    id int auto_increment primary key comment '主键ID',
    degree varchar(20) comment '学历',
    major varchar(50) comment '专业',
    primaryschool varchar(50) comment '小学',
    middleschool varchar(50) comment '中学',
    university varchar(50) comment '大学',
    userid int unique comment '用户ID', //唯一
    constraint fk_userid foreign key (userid) references tb_user(id)  //外键约束
) comment '用户教育信息表';

insert into tb_user(id, name, age, gender, phone) values
        (null,'黄渤',45,'1','18800001111'),
        (null,'冰冰',35,'2','18800002222'),
        (null,'码云',55,'1','18800008888'),
        (null,'李彦宏',50,'1','18800009999');

insert into tb_user_edu(id, degree, major, primaryschool, middleschool, university, userid) values
        (null,'本科','舞蹈','静安区第一小学','静安区第一中学','北京舞蹈学院',1),
        (null,'硕士','表演','朝阳区第一小学','朝阳区第一中学','北京电影学院',2),
        (null,'本科','英语','杭州市第一小学','杭州市第一中学','杭州师范大学',3),
        (null,'本科','应用数学','阳泉第一小学','阳泉区第一中学','清华大学',4);
//多表查询
//删除emp
drop table if exists emp;
//创建表
create table dept(
    id   int auto_increment comment 'ID' primary key,
    name varchar(50) not null comment '部门名称'
)comment '部门表';

create table emp(
    id  int auto_increment comment 'ID' primary key,
    name varchar(50) not null comment '姓名',
    age  int comment '年龄',
    job varchar(20) comment '职位',
    salary int comment '薪资',
    entrydate date comment '入职时间',
    managerid int comment '直属领导ID',
    dept_id int comment '部门ID'
)comment '员工表';
 //添加外键
 alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);
//添加数据
insert into dept(id,name) values (1, '研发部'), (2, '市场部'),(3, '财务部'), (4, '销售部'), (5, '总经办'), (6, '人事部');
insert into emp (id, name, age, job,salary, entrydate, managerid, dept_id) values
            (1, '金庸', 66, '总裁',20000, '2000-01-01', null,5),

            (2, '张无忌', 20, '项目经理',12500, '2005-12-05', 1,1),
            (3, '杨逍', 33, '开发', 8400,'2000-11-03', 2,1),
            (4, '韦一笑', 48, '开发',11000, '2002-02-05', 2,1),
            (5, '常遇春', 43, '开发',10500, '2004-09-07', 3,1),
            (6, '小昭', 19, '程序员鼓励师',6600, '2004-10-12', 2,1),

            (7, '灭绝', 60, '财务总监',8500, '2002-09-12', 1,3),
            (8, '周芷若', 19, '会计',48000, '2006-06-02', 7,3),
            (9, '丁敏君', 23, '出纳',5250, '2009-05-13', 7,3),

            (10, '赵敏', 20, '市场部总监',12500, '2004-10-12', 1,2),
            (11, '鹿杖客', 56, '职员',3750, '2006-10-03', 10,2),
            (12, '鹤笔翁', 19, '职员',3750, '2007-05-09', 10,2),
            (13, '方东白', 19, '职员',5500, '2009-02-12', 10,2),

            (14, '张三丰', 88, '销售总监',14000, '2004-10-12', 1,4),
            (15, '俞莲舟', 38, '销售',4600, '2004-10-12', 14,4),
            (16, '宋远桥', 40, '销售',4600, '2004-10-12', 14,4),
            (17, '陈友谅', 42, null,2000, '2011-10-12', 1,null);

//多表查询  笛卡尔积
select * from emp,dept;
//消除笛卡尔积
select * from emp,dept where emp.dept_id=dept.id;
//内连接  
//查询每一个员工的姓名 , 及关联的部门的名称 (隐式内连接实现)
//隐式内连接
//表结构 emp dept
// 连接条件 emp.dept_id=dept.id;
select emp.name,dept.name from emp,dept where emp.dept_id=dept.id;
//别名
select e.name, d.name from emp e,dept d where e.dept_id=d.id;

//显示内连接 inner join
select emp.name,dept.name from emp inner join dept on emp.dept_id=dept.id;
//-- 1. 查询emp表的所有数据, 和对应的部门信息(左外连接) outer 可以省
//-- 表结构: emp, dept
//-- 连接条件: emp.dept_id = dept.id

select e.*, d.name from emp e left outer join dept d on e.dept_id = d.id;

select e.*, d.name from emp e left join dept d on e.dept_id = d.id;


//-- 2. 查询dept表的所有数据, 和对应的员工信息(右外连接)  换一下位置

select d.*, e.* from emp e right outer join dept d on e.dept_id = d.id;

select d.*, e.* from dept d left outer join emp e on e.dept_id = d.id;



//自连接
//-- 1. 查询员工 及其 所属领导的名字
//-- 表结构: emp  //必须用别名

select a.name , b.name from emp a , emp b where a.managerid = b.id;

//-- 2. 查询所有员工 emp 及其领导的名字 emp , 如果员工没有领导, 也需要查询出来 外连接
//-- 表结构: emp a , emp b

select a.name '员工', b.name '领导' from emp a left join emp b on a.managerid = b.id;


//联合查询

//-- union all , union
//-- 1. 将薪资低于 5000 的员工 , 和 年龄大于 50 岁的员工全部查询出来.
select * from emp where salary <5000
union all
select * from emp where age >50;
//去重 不要 all
select * from emp where salary <5000
union 
select * from emp where age >50;
//子查询
//-- 标量子查询
//-- 1. 查询 "销售部" 的所有员工信息
//-- a. 查询 "销售部" 部门ID
select id from dept where name = '销售部';
//-- b. 根据销售部部门ID, 查询员工信息
select * from emp where dept_id = (select id from dept where name = '销售部');


//-- 2. 查询在 "方东白" 入职之后的员工信息
//-- a. 查询 方东白 的入职日期
select entrydate from emp where name = '方东白';

//-- b. 查询指定入职日期之后入职的员工信息
select * from emp where entrydate > (select entrydate from emp where name = '方东白');


//-- 列子查询
//-- 1. 查询 "销售部" 和 "市场部" 的所有员工信息
//-- a. 查询 "销售部" 和 "市场部" 的部门ID  in  not in    some any all
select id from dept where name = '销售部' or name = '市场部';

//-- b. 根据部门ID, 查询员工信息
select * from emp where dept_id in (select id from dept where name = '销售部' or name = '市场部');


//-- 2. 查询比 财务部 所有人工资都高的员工信息
//-- a. 查询所有 财务部 人员工资
select id from dept where name = '财务部';

select salary from emp where dept_id = (select id from dept where name = '财务部');

//-- b. 比 财务部 所有人工资都高的员工信息
select * from emp where salary > all ( select salary from emp where dept_id = (select id from dept where name = '财务部') );

//-- 3. 查询比研发部其中任意一人工资高的员工信息
//-- a. 查询研发部所有人工资
select salary from emp where dept_id = (select id from dept where name = '研发部');

//-- b. 比研发部其中任意一人工资高的员工信息
select * from emp where salary > some ( select salary from emp where dept_id = (select id from dept where name = '研发部') );

//-- 行子查询
//-- 1. 查询与 "张无忌" 的薪资及直属领导相同的员工信息 ;
//-- a. 查询 "张无忌" 的薪资及直属领导
select salary, managerid from emp where name = '张无忌';

//-- b. 查询与 "张无忌" 的薪资及直属领导相同的员工信息 ;
select * from emp where (salary,managerid) = (select salary, managerid from emp where name = '张无忌');

//-- 表子查询
//-- 1. 查询与 "鹿杖客" , "宋远桥" 的职位和薪资相同的员工信息
//-- a. 查询 "鹿杖客" , "宋远桥" 的职位和薪资
select job, salary from emp where name = '鹿杖客' or name = '宋远桥';

//-- b. 查询与 "鹿杖客" , "宋远桥" 的职位和薪资相同的员工信息
select * from emp where (job,salary) in ( select job, salary from emp where name = '鹿杖客' or name = '宋远桥' );


//-- 2. 查询入职日期是 "2006-01-01" 之后的员工信息 , 及其部门信息
//-- a. 入职日期是 "2006-01-01" 之后的员工信息
select * from emp where entrydate > '2006-01-01';

//-- b. 查询这部分员工, 对应的部门信息; 子查询结果 作为表 连查
select e.*, d.* from (select * from emp where entrydate > '2006-01-01') e left join dept d on e.dept_id = d.id ;


//事务
create table account(
    id int auto_increment primary key comment '主键ID',
    name varchar(10) comment '姓名',
    money int comment '余额'
) comment '账户表';
insert into account(id, name, money) VALUES (null,'张三',2000),(null,'李四',2000);

//-- 转账操作 (张三给李四转账1000)
//-- 1. 查询张三账户余额
select * from account where name = '张三';

//-- 2. 将张三账户余额-1000
update account set money = money - 1000 where name = '张三';

//-- 3. 将李四账户余额+1000
update account set money = money + 1000 where name = '李四';
//事务 控制
//方式一 手动提交
select @@autocommit;
set @@autocommit = 0; -- 设置为手动提交
//-- 转账操作 (张三给李四转账1000)
//-- 1. 查询张三账户余额
select * from account where name = '张三';
//-- 2. 将张三账户余额-1000
update account set money = money - 1000 where name = '张三';
程序执行报错 ...
//-- 3. 将李四账户余额+1000
update account set money = money + 1000 where name = '李四';
//- 提交事务
commit;
//-- 回滚事务
rollback ;
//方式二
//-- 方式二  transaction
//-- 转账操作 (张三给李四转账1000)
start transaction ;
//-- 1. 查询张三账户余额
select * from account where name = '张三';
//-- 2. 将张三账户余额-1000
update account set money = money - 1000 where name = '张三';
程序执行报错 ...
//-- 3. 将李四账户余额+1000
update account set money = money + 1000 where name = '李四';
//-- 提交事务
commit;
//-- 回滚事务
rollback;



//-- 查看事务隔离级别
select @@transaction_isolation;

//-- 设置事务隔离级别
set session transaction isolation level read uncommitted ;

set session transaction isolation level repeatable read ;

//创建索引
show index from tb_user1;//查看索引

create index idx_user_name on tb_user1(name);
create unique index idx_user_phone on tb_user1(phone); //唯一
create index idx_user_pro_age_sta on tb_user1(profession,age,status);
create index idx_user_email on tb_user1(email);
//删除 索引
drop index idx_user_email on tb_user1;
//性能分析
show global status like 'Com%';
show global status like 'Com_______';//执行频次
//慢查询日志
show variables like 'slow_query_log';
//profile 详情
select @@have_profiling;
select @@profiling;
set profiling =1;
show profile for query query_id;
show profile cpu for query query_id;
//查看执行计划
explain select * from tb_user1 where id =1;
desc select * from tb_user1 where id =1;

//多表查询
select s.*,c.* from student s, course c,student_course sc where s.id=sc.studentid and c.id=sc.courseid;
desc select s.*,c.* from student s, course c,student_course sc where s.id=sc.studentid and c.id=sc.courseid;
//子查询
select * from student s where s.id in 
(select studentid from student_course sc where sc.courseid=
(select id from course c where c.name='MySQL'
));
desc select * from student s where s.id in 
(select studentid from student_course sc where sc.courseid=
(select id from course c where c.name='MySQL'
));
//索引使用  最左前缀法则  联合索引  跟放的位置无关
select * from tb_user1 where profession ='软件工程' and age=31 and status ='0';
desc select * from tb_user1 where profession ='软件工程' and age=31 and status ='0';
select * from tb_user1 where profession ='软件工程' and age>31 and status ='0';//范围查询
desc select * from tb_user1 where profession ='软件工程' and age>30 and status ='0'; 
//允许条件下尽量用>=，否则>后索引实效
desc select * from tb_user1 where substring(phone,10,2)='15';//函数运算 索引失效
desc select * from tb_user1 where phone=1000000000000;//字符串不加引号 失效
desc select * from tb_user1 where phone='%yu';//模糊匹配 前面模糊 索引失效
desc select * from tb_user1 where age=23 or id=10;//or 两个都有索引 才能用索引
create index idex_user_age on tb_user1(age);
//sql 提示
select * from tb_user1 use index(idx_user_pro) where profession = '软件工程';
//覆盖索引
//索引覆盖返回项目了就不用回表   index 
//index condition 回表  主键不管 都挂了
//前缀索引
select count(*) from tb_user1; //所有
select count(distinct email) from tb_user1; //email 去重
//选择性计算
select count(distinct substring(email,1,7))/count(*) from tb_user1;
 create index idx_email_5 on tb_user1(email(5));//取email前5个字段
//**************************************************************************
//优化
//insert 优化  批量插入 手动提交事务 主键顺序插入
//load 本地数据文件
mysql --local-infile -u root -p
set global local_infile =1;
load data local infile 'path'


//视图
//创建
create or replace view stu_v_1 as select id ,name from student where id <=10;
//查询
show create view stu_v_1;
select * from stu_v_1;
//修改
create or replace view stu_v_1 as select id ,name,no from student where id <=10;
alter view stu_v_1 as select id ,name,no from student where id <=10;
//删除
drop view if exists stu_v_1;

//检查选项  
create or replace view stu_v_1 as select id ,name from student where id <=10 with cascaded/local check option;
//检查 视图增删改查是否合理

// view update    can't update  sum min max count distinct group by having union union all
create or replace view stu_v_count as select count(*) from student where id>5;


//创建存储过程

create procedure p1() 
//begin()
    select count(*) from student;
//end;

//命令行
delimiter $$ //修改结束标志
create procedure p1() 
begin()
    select count(*) from student;
end$$
//调用
call p1();
//查看
 select * from information_schema.ROUTINES where ROUTINE_SCHEMA = 'mytest';
 show create procedure p1;
//删除
drop procedure if exists p1;



//存储过程 
//变量  global  session 
show session /global  variables; //查看变量
show global variables like 'auto%';
select @@global.autocommit;
 //设置变量
 set session autocommit=0; //自动提交
 //自定义变量
 set @myname ='yjh'; //不用声明
 set @myage:=10;
 set @mygender:='男',@myhobby:='java';

 //赋值
 select count(*) into @mycount from tb_user1;
 select @mycolor:='red';
 //查看 
 select  @mycolor;

 //局部变量 声明
 delimiter $$
create procedure p2()
begin
     declare stu_count int default 0;
     set stu_count :=100;
     select count(*) into stu_count from student;
     select stu_count;
end$$
//条件判断  if
delimiter $$
create procedure p3()
begin
  declare score int default 58;
  declare result varchar(10);
  if score>=85 then
     set result:='优秀';
elseif score>=60 then
    set result:='及格';
  else 
  set result :='不及格';
  end if;
  select result;
end$$
 
//参数
delimiter $$
create procedure p4(in score int, out result varchar(10))
begin 
  if score>=85 then
     set result:='优秀';
elseif score>=60 then
    set result:='及格';
  else 
  set result :='不及格';
  end if;
end$$
// 调用
call p4(98,@result);
select @result;


delimiter $$
create procedure p5(inout score double)
begin 
   set score:=score*0.5;
end$$
// 传入传出参数 调用
set @score :=78;
call p5(@score);
select @score;
  

  //case 过程

delimiter $$
create procedure p6(in month int)
begin 
  declare result varchar(10);
  case 
      when month>=1 and month<=3 then
       set result :='first season';
        when month>=4 and month<=6 then
       set result :='second season';
        when month>=7 and month<=9 then
       set result :='third season';
        when month>=10 and month<=12 then
       set result :='forth season';
       else 
           set result:='error';
    end case;
    select concat('month:',month,'season:',result);
end$$
//while 循环
delimiter $$
create procedure p7(in n int)
begin
    declare total int default 0;

    while n>0 do
         set total := total + n;
         set n := n - 1;
    end while;

    select total;
end$$
//repeat until
delimiter $$
create procedure p8(in n int)
begin
    declare total int default 0;

    repeat
        set total := total + n;
        set n := n - 1;
    until  n <= 0
    end repeat;

    select total;
end$$
// loop 循环
delimiter $$
create procedure p9(in n int)
begin
    declare total int default 0;

    sum:loop
        if n<=0 then
            leave sum;
        end if;

        set total := total + n;
        set n := n - 1;
    end loop sum;

    select total;
end$$


delimiter $$
create procedure p10(in n int)
begin
    declare total int default 0;

    sum:loop
        if n<=0 then
            leave sum;
        end if;
        if n%2=0 then
        set n := n - 1;
            iterate sum;
        end if;
        set total := total + n;
        set n := n - 1;
    end loop sum;
    select total;
end$$
 //游标使用
// -- 游标
// -- 根据传入的参数uage，来查询用户表 tb_user中，所有的用户年龄小于等于uage的用户姓名（name）和专业（profession），
// -- 并将用户的姓名和专业插入到所创建的一张新表(id,name,profession)中。
// -- 逻辑:
// -- A. 声明游标, 存储查询结果集
// -- B. 准备: 创建表结构
// -- C. 开启游标
// -- D. 获取游标中的记录
// -- E. 插入数据到新表中
// -- F. 关闭游标
create procedure p11(in uage int)
begin
    declare uname varchar(100); 
    declare upro varchar(100);
    declare u_cursor cursor for select name,profession from tb_user where age <= uage;
    declare exit handler for SQLSTATE '02000' close u_cursor;

    drop table if exists tb_user_pro;
    create table if not exists tb_user_pro(
        id int primary key auto_increment,
        name varchar(100),
        profession varchar(100)
    );
    open u_cursor;
    while true do
        fetch u_cursor into uname,upro;
        insert into tb_user_pro values (null, uname, upro);
    end while;
    close u_cursor;

end$$


create procedure p12(in uage int)
begin
    declare uname varchar(100);
    declare upro varchar(100);
    declare u_cursor cursor for select name,profession from tb_user1 where age <= uage;
    declare exit handler for not found close u_cursor;

    drop table if exists tb_user_pro;
    create table if not exists tb_user_pro(
        id int primary key auto_increment,
        name varchar(100),
        profession varchar(100)
    );

    open u_cursor;
    while true do
        fetch u_cursor into uname,upro;
        insert into tb_user_pro values (null, uname, upro);
    end while;
    close u_cursor;

end$$


//存储函数
create function fun1(n int)
returns int deterministic
begin
    declare total int default 0;

    while n>0 do
        set total := total + n;
        set n := n - 1;
    end while;

    return total;
end;
//调用
select fun1(10);

// -- 触发器
// -- 需求: 通过触发器记录 user 表的数据变更日志(user_logs) , 包含增加, 修改 , 删除 ;

// -- 准备工作 : 日志表 user_logs
create table user_logs(
  id int(11) not null auto_increment,
  operation varchar(20) not null comment '操作类型, insert/update/delete',
  operate_time datetime not null comment '操作时间',
  operate_id int(11) not null comment '操作的ID',
  operate_params varchar(500) comment '操作参数',
  primary key(`id`)
)engine=innodb default charset=utf8;

-- 插入数据触发器
delimiter $$ //创建触发器
create trigger tb_user_insert_trigger
    after insert on tb_user1 for each row //行触发器
begin
    insert into user_logs(id, operation, operate_time, operate_id, operate_params) VALUES
    (null, 'insert', now(), new.id, concat('插入的数据内容为: id=',new.id,',name=',new.name, ', phone=', NEW.phone, ', email=', NEW.email, ', profession=', NEW.profession));
end$$


-- 查看
show triggers ;

-- 删除
drop trigger tb_user_insert_trigger;

-- 插入数据到tb_user
insert into tb_user(id, name, phone, email, profession, age, gender, status, createtime) VALUES (26,'三皇子','18809091212','erhuangzi@163.com','软件工程',23,'1','1',now());



-- 修改数据触发器
create trigger tb_user_update_trigger
    after update on tb_user for each row
begin
    insert into user_logs(id, operation, operate_time, operate_id, operate_params) VALUES
    (null, 'update', now(), new.id,
        concat('更新之前的数据: id=',old.id,',name=',old.name, ', phone=', old.phone, ', email=', old.email, ', profession=', old.profession,
            ' | 更新之后的数据: id=',new.id,',name=',new.name, ', phone=', NEW.phone, ', email=', NEW.email, ', profession=', NEW.profession));
end;

show triggers ;

update tb_user set profession = '会计' where id = 23;

update tb_user set profession = '会计' where id <= 5;





-- 删除数据触发器
create trigger tb_user_delete_trigger
    after delete on tb_user for each row
begin
    insert into user_logs(id, operation, operate_time, operate_id, operate_params) VALUES
    (null, 'delete', now(), old.id,  //删除之前的数据
        concat('删除之前的数据: id=',old.id,',name=',old.name, ', phone=', old.phone, ', email=', old.email, ', profession=', old.profession));
end;

show triggers ;


delete from tb_user where id = 26;
















































































