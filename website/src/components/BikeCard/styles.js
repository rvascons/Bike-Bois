import styled from "styled-components";

const Container = styled.div`
	background: #fafafa;
	box-shadow: 0px 0px 4px rgba(0, 0, 0, 0.25);
	border-radius: 5px;
	margin: 30px 0;
	height: 150px;
	width: 100%;
	display: flex;
	flex-direction: column;
`;

const Infos = styled.div`
	display: flex;
	flex: 1;
	padding: 15px 20px;
`;

const Model = styled.div`
	font-style: normal;
	font-weight: 500;
	font-size: 16px;
	line-height: 19px;
	flex: 2;
	height: 100%;
	display: flex;
	flex-direction: column;
	justify-content: center;
`;

const Name = styled.p`
	font-style: normal;
	font-weight: 600;
	font-size: 20px;
	line-height: 24px;
`;

const Color = styled.p`
	font-style: normal;
	font-weight: 400;
	font-size: 20px;
	line-height: 24px;
`;

const Locator = styled(Color)``;

const ImageContainer = styled.div`
	flex: 1;
	display: flex;
	justify-content: center;
	align-items: center;
`;

const Image = styled.img`
	max-height: 60px;
`;

const Status = styled.div`
	display: flex;
	justify-content: center;
	align-items: center;
	height: 45px;
	width: 100%;
	background-color: ${({ inUse }) => (inUse ? "#FF7438" : "#62FF68")};
	border-bottom-left-radius: 5px;
	border-bottom-right-radius: 5px;
	transition: all 0.5s ease;
`;

const StatusText = styled.p`
	font-style: normal;
	font-weight: 500;
	font-size: 16px;
	line-height: 19px;
`;

export {
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
};
