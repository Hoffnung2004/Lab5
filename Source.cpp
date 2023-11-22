
//от user1 В этuser2user3user2 и user3, и, наконец, делаем твит от user1, ожидая автоматических ретвитов от user2 и user3.
#include <iostream>
#include <vector>
#include <functional>

class Account
{
public:
	explicit Account(int i_id) : m_id(i_id) {}

	void Tweet(const std::string& i_message) {
		std::string tweet = std::to_string(m_id) + ": " + i_message;
		m_tweets.push_back(tweet);

		// Уведомляем подписчиков
		NotifyFollowers(tweet);
	}

	void ReTweet(int i_id, const std::string& i_message) {
		std::string retweet = std::to_string(i_id) + ": " + i_message;
		m_tweets.push_back(retweet);
	}

	void AddFollower(const std::function<void(int, const std::string&)>& i_callback) {
		m_followers.push_back(i_callback);
	}

	void SetAutoReTweet(Account& io_leader) {
		auto retweetCallback = [this](int i_id, const std::string& i_message) {
			this->ReTweet(i_id, i_message);
			};
		io_leader.AddFollower(retweetCallback);
	}

	const std::vector<std::string>& GetAllTweets() const {
		return m_tweets;
	}

private:
	void NotifyFollowers(const std::string& tweet) {
		for (const auto& follower : m_followers) {
			follower(m_id, tweet);
		}
	}

	const int m_id;
	std::vector<std::string> m_tweets;
	std::vector<std::function<void(int, const std::string&)>> m_followers;
};

int main() {
	std::vector<Account> users;

	// Создаем пользователей
	Account user1(1);
	Account user2(2);
	Account user3(3);

	// Настройка автоматического ретвита
	user1.SetAutoReTweet(user2);
	user1.SetAutoReTweet(user3);

	// Добавляем подписчиков с выводом в консоль
	user1.AddFollower([&](int id, const std::string& tweet) {
		std::cout << "User " << id << " tweeted: " << tweet << std::endl;
		});

	user2.AddFollower([&](int id, const std::string& tweet) {
		std::cout << "User " << id << " tweeted: " << tweet << std::endl;
		});

	user3.AddFollower([&](int id, const std::string& tweet) {
		std::cout << "User " << id << " tweeted: " << tweet << std::endl;
		});

	// Твиты от пользователей
	user2.Tweet("Hello, Twitter!");
	user3.Tweet("I love coding!");

	// Твит от пользователя 1, должны произойти автоматические ретвиты
	user1.Tweet("Testing, testing!");

	// Вывод всех твитов каждого пользователя
	for (const auto& user : users) {
		const auto& tweets = user.GetAllTweets();
		for (const auto& tweet : tweets) {
			std::cout << tweet << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}