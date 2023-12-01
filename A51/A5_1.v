module A5_1 (
    input clk, 
    input reset, 
    input [63:0] key, 
    input [63:0] plaintext,  // 新增的明文输入
    output wire z, 
    output reg [63:0] ciphertext  // 新增的密文输出
);

    reg [18:0] R1;
    reg [21:0] R2;
    reg [22:0] R3;
    
    assign z = R1[18] ^ R2[21] ^ R3[22];
    
always @(posedge clk or posedge reset) begin
    if (reset) begin
        R1 <= key[18:0];
        R2 <= key[39:19];
        R3 <= key[63:40];
        ciphertext <= 64'h0;  // 在复位时清空密文
    end else begin
        if ((R1[8] ^ R2[10] ^ R3[10]) == 1'b1) begin
            R1 <= {R1[13] ^ R1[16] ^ R1[17] ^ R1[18], R1[17:0]};
        end
        if ((R2[10] ^ R1[8] ^ R3[10]) == 1'b1) begin
            R2 <= {R2[20] ^ R2[21], R2[20:0]};
        end
        if ((R3[10] ^ R1[8] ^ R2[10]) == 1'b1) begin
            R3 <= {R3[7] ^ R3[20] ^ R3[21] ^ R3[22], R3[21:0]};
        end
        ciphertext <= plaintext ^ {64{z}};  // 更新密文
    end
end
endmodule