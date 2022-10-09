import styled from "styled-components";

const Container = styled.footer`
	width: 100%;
	height: 60px;
	background-color: #00d1ff;
	display: flex;
	justify-content: center;
	align-items: center;
`;

const Image = styled.img`
	height: 60%;
`;

const Text = styled.span`
	font-style: normal;
	font-weight: 600;
	font-size: 14px;
	margin-left: 10px;
`;

export { Container, Image, Text };
