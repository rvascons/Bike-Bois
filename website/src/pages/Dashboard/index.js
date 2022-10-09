import Header from "../../components/Header";
import Title from "../../components/Title";
import { Container, Content } from "./styles";
import { useContext } from "react";
import { FirestoreContext } from "../../contexts/Firestore";
import BikeCard from "../../components/BikeCard";

const Dashboard = () => {
	const {
		firebaseData: { bikes },
	} = useContext(FirestoreContext);

	return (
		<Container>
			<Header />
			<Content>
				<Title text="STATUS DAS BIKES" />
				{bikes.map((bike, index) => (
					<BikeCard key={index} bike={bike} />
				))}
			</Content>
		</Container>
	);
};

export default Dashboard;
