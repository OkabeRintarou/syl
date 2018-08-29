#ifndef _BITTORRENT_H_
#define _BITTORRENT_H_

#include <string>
#include <vector>

/*
 * Multi - file Torrent
 * ├─announce
 * ├─announce - list
 * ├─comment
 * ├─comment.utf - 8
 * ├─creation date
 * ├─encoding
 * ├─info
 * │ ├─files
 * │ │ ├─length
 * │ │ ├─path
 * │ │ └─path.utf - 8
 * │ ├─name
 * │ ├─name.utf - 8
 * │ ├─piece length
 * │ ├─pieces
 * │ ├─publisher
 * │ ├─publisher - url
 * │ ├─publisher - url.utf - 8
 * │ └─publisher.utf - 8
 * └─nodes
**/


/* Single-File-Torrent
 * | announce
 * |- announce-list
 * |- comment
 * |- comment.utf-8
 * |- creation date
 * |- encoding
 * |- info
 * |  |- length
 * |  |- name
 * |  |- name.utf-8
 * |  |- pieces
 * |  |- publisher
 * |  |- publisher-url
 * |  |- publisher-url-utf-8
 * |  |- publisher.utf-8
 * |_ nodes
**/

class Info
{
public:
	Info()
	{
		files = 0;
		length = 0;
	}

	~Info()
	{
		if (files){
			delete[] files;
		}
	}
private:
	friend class BitTorrent;
	struct File
	{
		unsigned long length;
		std::string path;
		std::string path_utf_8;
	};
private:
	
	File * files;
	unsigned long length;

	std::string name;
	std::string name_utf_8;
	std::string publisher;
	std::string publisher_utf_8;
	std::string publisher_url;
	std::string publisher_url_utf_8;
};

class BitTorrent
{
public:
	BitTorrent(std::string filename);

	std::string getAnnounce(){ return announce_; }
	const std::string & getAnnounce()const{ return announce_; }
	std::vector<std::string> getAnnounceList(){ return announce_list_; }
	const std::vector<std::string> & getAnnounceList()const{ return announce_list_; }
	std::string getFileName(){ return info_.name; }
	const std::string & getFileName()const{return info_.name;}
	std::string getComment(){ return comment_; }
	const std::string & getComment()const{ return comment_; }
	unsigned long getCreationDate()const{ return creation_date_; }
	unsigned int getPieceLength()const{ return piece_length_; }
	unsigned long getFileLength()const{ return this->info_.length; }
	std::string getPieces(){ return pieces_; }
	const std::string & getPieces()const{ return pieces_; }
private:
	std::string announce_;						// Tracker主服务器
	std::vector<std::string> announce_list_;    // Tracker服务器列表
	std::string comment_;						// 种子文件的注释
	std::string comment_utf_8_;				
	unsigned long creation_date_;
	unsigned int piece_length_;
	std::string pieces_;
	std::string encoding_;
	Info info_;
	std::string node_;							// 一系列ip和相应端口的列表
};


#endif