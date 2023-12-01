`timescale 1ns / 1ps

module A5_1_test;
    reg clk;
    reg reset;
    reg [63:0] key;
    reg [63:0] plaintext;  // 新增明文输入
    wire z;
    wire [63:0] ciphertext;  // 新增密文输出

    // 实例化A5/1模块
    A5_1 u1 (.clk(clk), .reset(reset), .key(key), .plaintext(plaintext), .z(z), .ciphertext(ciphertext));

    initial begin
        // 初始化输入信号
        clk = 0;
        reset = 0;
        key = 64'h123456789ABCDEF0;
        plaintext = 64'hFEDCBA9876543210;  // 初始化明文
       
        // 指定VCD文件的名称并打开它
        $dumpfile("A5_1_test.vcd");
        $dumpvars(0, A5_1_test);
        
        // 重置A5/1模块
        reset = 1;
        #10 reset = 0;
        
        // 生成时钟信号并观察输出
        repeat (100) begin
            #10 clk = ~clk;
            plaintext = {plaintext[62:0], plaintext[63]};  // 将明文左移一位
            $display("At time %t, z = %b, ciphertext = %h", $time, z, ciphertext);
        end
        
        // 结束仿真
        $finish;
    end
    
    always #5 clk = ~clk;
endmodule