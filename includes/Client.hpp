class Client {
private:
	const int fd;
public:
	Client();
	Client(int fd);
	~Client();

	void disconnect();

	/**
	* 
	*/
	bool handleMsg();
};
