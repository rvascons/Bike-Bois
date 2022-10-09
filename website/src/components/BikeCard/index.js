import {
	Container,
	Infos,
	Status,
	StatusText,
	Model,
	ImageContainer,
	Image,
	Name,
	Color,
	Locator,
} from "./styles";

const BikeCard = ({ bike }) => {
	const { name, inUse, image, color, tenant } = bike;
	return (
		<Container>
			<Infos>
				<Model>
					<Name>{name}</Name>
					<Color>COR: {color}</Color>
					{inUse && <Locator>LOCATÁRIO: {tenant}</Locator>}
				</Model>
				<ImageContainer>
					<Image src={image} alt={name} />
				</ImageContainer>
			</Infos>
			<Status inUse={inUse}>
				<StatusText>{inUse ? "EM USO" : "DISPONÍVEL PARA USO"}</StatusText>
			</Status>
		</Container>
	);
};

export default BikeCard;
