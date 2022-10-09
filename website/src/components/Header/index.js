import { Container, Image, Text } from "./styles";
import logo from "../../assets/images/bike-logo.png";

const Header = () => {
	return (
		<Container>
			<Image src={logo} alt="Logo" />
			<Text>BOIS</Text>
		</Container>
	);
};

export default Header;
