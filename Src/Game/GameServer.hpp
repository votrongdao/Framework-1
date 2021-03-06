#pragma once

#include <map>
#include <vector>

#include <Network/Server.h>
#include <Network/Packet.h>

#include <Game/Player.hpp>
#include <Game/GOMDatabase.hpp>

namespace Game
{
	class GameServer
	{
	public:

		typedef std::function<Player* (Player::KeyType, GameServer*)> PlayerConstructor;
		typedef std::function<std::vector<IGOMServer*> (void*)>	GOMServerConstructor;

		/**
		 * @brief Constructs a GameServer instance
		 * @param port The port on which the server will listen
		 * @param The functor handling player construction
		 * @param The functor handling GOM Server construction
		 */
		GameServer(short port, PlayerConstructor, GOMServerConstructor);
		~GameServer();

		/**
		 * @brief Update's the GameServer state.
		 */
		void Update();

		/**
		 * @brief Send a message to every player currently connected to the game server.
		 * @param pPacket The packet to send.
		 */
		void SendMessageAll(Framework::Network::Packet& pPacket);

		/**
		 * @brief Send a message to every player currently connected to the game server and synchronized.
		 * @param pPacket The packet to send.
		 */
		void SendMessageAllSynchronized(Framework::Network::Packet& pPacket);

		/**
		 * @brief Sets the size of the GOM's view distance.
		 * @param cellsize The size of the GOM's view distance.
		 */
		void SetCellSize(int cellsize);

		/**
		 * @brief Gets the size of the GOM's view distance.
		 * @return The size of the GOM's view distance.
		 */
		int GetCellSize() const;

		/**
		 * @brief Callback handling new client connections to the GameServer.
		 * @param pConnection The new connection.
		 */
		void OnConnection(Framework::Network::TcpConnection::pointer pConnection);

		/**
		 * @brief Remove a player from the server.
		 * @param pPlayer The player to remove.
		 */
		void Remove(Player* pPlayer);

		/**
		 * @brief Retrieves a player by key.
		 * @param pKey The player's key.
		 * @return The player if found, nullptr otherwise.
		 */
		Player* GetPlayer(Player::KeyType pKey);

	private:

		void SendReplicationTransaction(GOMVisitor& visitor);

		std::unique_ptr<Framework::Network::Server>			mServer;

		boost::timer mTransactionFullTimer;
		boost::timer mTransactionPartialTimer;

		boost::recursive_mutex mLock;
		std::map<Player::KeyType, Player*> mPlayers;

		std::list<Player*> mToRemove;

		int mCellSize;

		friend class MassiveMessageManager;

		PlayerConstructor		mPlayerContructor;
	};
}