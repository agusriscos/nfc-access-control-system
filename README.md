# My TFG: Design and implementation of an access control system based on NFC technology

The B105 Electronic Systems Lab has an electronic access system in its door based on a Radio Frequency Identification (RFID) card reader. This system was developed more than 12 years ago so the technology it uses is obsolete and several of its features are out of use. The development of this degree project is intended to implement an alternative to this access control system based on Near Field Communication (NFC) technology.

The RFID system requires the use of physical cards, which are easily misplaced and force the user to carry them around with him/her to enter the laboratory. To solve this problem, the new system allows the users to open the door using their smartphone. This makes it even easier to enter the laboratory, as users always have their mobile phone with them. In addition, users are assigned specific entry times, providing greater security and a better access control to the laboratory.

There is an equipment reservation management service in the laboratory that already has a database of members, an application and an administration website. Therefore, these resources have been used to facilitate the implementation of the new system and avoid data replication on the server.

Once the system has been implemented, any user who is registered in the system and has certain permissions can open the door by bringing their mobile phone closer to the reader. To achieve this, the existing access system has been built on and relevant technologies have been studied.

The development and implementation work has been divided into three blocks: the NFC reader, the application and the server. The reader, integrated into the door opening system, acts as an intermediary between the application and the server. On the other hand, the application only has to emulate the access card and send the entry request. Then, the server evaluates this request checking the user information and its database and it sends a response to the reader. Depending on the message received, the reader opens the door or not and finally informs the user of the decision.
