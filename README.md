# Minifilter: Control Delete, Move and Rename Operations

[Silme İşlemlerinin Kontrolü](https://app.clickup.com/9018044784/v/dc/8cr8wbg-10318/8cr8wbg-20858) senaryosu gerçeklenmiştir.\
Minifilter, attach olduğu volume'de gerçekleşen `DELETE`, `MOVE` ve `RENAME` işlemlerini user onayı sunmakta, gelen cevaba göre işlemi engellemekte ya da gerçekleşmesine izin vermektedir.

> **NOTE:** User programı çalışmıyorken minifilter attach olduğu volume'deki `DELETE`, `MOVE` ve `RENAME` işlemlerini direkt olarak engeller.
> ~~cmd.exe'nin `DELETE` işlemini minifilter'a rağmen yapabildiği görülmüştür. Bu durum ayrı olarak incelenmelidir.~~
> IRP_MJ_CREATE major fonksiyonu register edilip "FILE_DELETE_ON_CLOSE" bayrağı kontrol edilerek **cmd.exe** ve **shift + delete** ile silme işlemleri de kontrol edilebilir hale getirilmiştir.

> **NOTE:** Kernel `STATUS_UNSUCCESSFUL` yerine `STATUS_ACCESS_DENIED` döndürdüğünde, user'ın `DELETE`, `MOVE` ve `RENAME` işlemleri için iki kere **ret** cevabı vermesi gerekmektedir.
> Vscode **ret** cevabı aldığında 9 kere tekrar sormaktadır.


# Screen Dumps

# DELETE and DENY the operation
![delete](https://github.com/user-attachments/assets/b3a509db-1ea0-43ba-a087-c52e5828f533)
![delete-deny](https://github.com/user-attachments/assets/d88120e8-72a4-4b11-ae17-1a16ee7f4bdd)

# VScode:
DELETE and DENY the operation\
![delete-vscode](https://github.com/user-attachments/assets/545de31b-d822-4432-a3ed-0ebfad15dba9)
![delete-vscode-deny](https://github.com/user-attachments/assets/5c3892f2-1e37-428b-805d-31383149d0f2)

# Notepad++
DELETE and DENY the operation\
![delete-notepad](https://github.com/user-attachments/assets/5e39f299-9111-4fc2-9bc5-7368365267ec)
![delete-notepad-dny](https://github.com/user-attachments/assets/639cdc6b-6ce4-45bb-9639-197ca6f73c93)


# MOVE and DENY the operation
![move](https://github.com/user-attachments/assets/c1703292-13ef-4a35-8392-a3fd187f09ca)
![move-deny](https://github.com/user-attachments/assets/e550a77d-8085-4381-909d-37f0e30357dc)

# VScode:
MOVE and DENY the operation\
![move-vscode](https://github.com/user-attachments/assets/d7dce562-baa7-48c8-b1df-8c96057cc427)
![move-vscode-deny](https://github.com/user-attachments/assets/fe21b219-35b2-415e-933e-ad3cc029813d)


# RENAME and DENY the operation
![rename](https://github.com/user-attachments/assets/4724183d-589c-4a73-abc0-6c2e605ef464)
![rename-deny](https://github.com/user-attachments/assets/03a6b335-ba7a-4c65-9c95-11da70f7804f)

# VScode:
RENAME and DENY the operation\
![rename-vscode](https://github.com/user-attachments/assets/987dd4d2-4d06-4c16-b352-cca51cf45c24)
![rename-vscode-deny](https://github.com/user-attachments/assets/7a00be6f-aa3b-423f-a683-bd7dd8176683)

# Notepad++
RENAME and DENY the operation\
![rename-notepad-deny](https://github.com/user-attachments/assets/0dd34a8d-44df-466c-9826-37089c9ea4cd)





