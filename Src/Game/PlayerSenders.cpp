#include "Player.hpp"
#include <Game/GOMDatabase.hpp>
#include <System/Log.h>

namespace Game
{
	void Player::SendReplicationTransaction(GOMDatabase& gomDatabase)
	{
		Framework::Network::Packet packet(kReplicationTransaction);

		GOMVisitor visitor;
		gomDatabase.VisitAll(Game::GOMDatabase::kAllGOMServers, visitor);

		visitor.apply([](GOMState& state){state.full = true;});

		packet << visitor.gomEntries;

		Write(packet);
	}
}