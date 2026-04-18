Đề bài trò chơi con rắn nhóm CC
- Ông A là một người yêu thích trò chơi điện tử. Một ngày nọ, khi đang hoài niệm về tuổi thơ, ông chợt nhớ đến trò chơi con rắn từng gắn bó với chiếc điện thoại cũ. Tuy nhiên, do không biết gì về lập trình nên ông đã đăng trên mạng xã hội, nhờ mọi người thiết kế trò chơi con rắn chạy được trên máy tính Windows dưới dạng console app. Theo mô tả của ông A, trò chơi có các thông tin cơ bản sau:
+	Tên người chơi
+	Điểm hiện tại của lượt đang chơi
+	Điểm cao nhất đạt được
+	Tổng điểm của tất cả lượt chơi
+	Số lượt đã chơi
+	Rắn, thức ăn, sân chơi
- Trong game có hai đối tượng là rắn và thức ăn xuất hiện trên sân. Đối với mỗi loại đối tượng có các đặc tính khác nhau:
+	Đối với thức ăn: hiển thị thức ăn trên sân với kí hiệu “@”, đặt thức ăn ngẫu nhiên, thức ăn không xuất hiện trùng với rắn, hàng rào.
+	Đối với rắn: hiển thị rắn trên sân với kí hiệu “O” và “o”, rắn sẽ di chuyển theo hướng điều khiển, khi đầu rắn va chạm với tường, với thân thì sẽ chết, sau mỗi lần ăn thức ăn thì người chơi được cộng 10 điểm.
+	Việc hiển thị rắn và thức ăn phải trong sân chơi được bao quanh bởi kí tự “*” có kích thước 80 x 20 (kí tự)
- Ngoài ra để người chơi có thể tương tác với game cần phải có các chức năng dưới đây:
+	Người chơi sử dụng các phím W, A, S, D hoặc các phím mũi tên để rắn có thể di chuyển theo bốn hướng: lên - xuống - trái - phải
+	Nhập tên người chơi khi bắt đầu, hiển thị các thông số tên người chơi, điểm hiện tại, điểm cao nhất, tổng điểm, số lượt đã chơi theo thời gian thực.
+	Có hiển thị kết thúc trò chơi, chơi lại, thống kê điểm số. Khi rắn sống, các hoạt động di chuyển của rắn được diễn ra, sau mỗi lần ăn, người chơi được cộng 10 điểm. Khi rắn chết xuất hiện dòng chữ trên màn hình: 
“Game Over, [Tên người chơi] da ghi duoc [Số điểm hiện tại] diem!” 
“Ban co muon choi lai khong? Hay chon Co(c)/Khong(k)”
+	Nếu người chơi chọn “c” thì chơi lại trò chơi, nếu chọn “k” thì hiển thị thông số đã chơi:
“Thong ke tro choi cua [Tên người chơi]”
“So lan choi: [Số lần đã chơi]”
“Diem cao nhat: [Điểm cao nhất trong các lượt]”
“Tong diem: [Tổng điểm tất cả các lượt chơi]”
+	Trong trường hợp người chơi không nhập một trong hai kí tự trên thì hiển thị dòng chữ:
“Hay chon lai mot trong hai ki tu tren!”

Từ các mô tả trên, hãy giúp ông A làm trò chơi con rắn với các chức năng, cùng với các yêu cầu sau:
a)	Hãy khai báo thuộc tính, phương thức của các lớp và mối quan hệ giữa chúng bằng ngôn ngữ C++.
b)	Sử dụng các toán tử nhập xuất để nhập tên người chơi, hiển thị các thông số trong lúc chơi.
c)	Sử dụng tính kế thừa để tái sử dụng tối đa các thuộc tính, tính đa hình để vẽ các đối tượng trên sân.
