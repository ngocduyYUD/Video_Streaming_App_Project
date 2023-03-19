import React from "react";
import styled from "styled-components";
// Components
// Assets
import { useSelector } from "react-redux";
export default function Header() {
  const theme = useSelector((state) => state.colorMode.mode);
  return (
    <>
      <LeftSide className="flexCenter">
        <div
          style={{
            backgroundColor: theme === "light" ? "#F5F5F5" : "#fff",
            border: "1px solid #E5E5E5",
            padding: "40px 40px 40px 40px",
            borderRadius: "8px",
            marginBottom: "20px",
            marginLeft: "20vw",
            color: theme === "light" ? "#000000" : "#12a",
          }}
        >
          <h1 className="extraBold font60">We are Team 14.</h1>
          <HeaderP className="font13 semiBold">
            We are a team of 4 students from the Hanoi University of Science and
            Technology.
          </HeaderP>
        </div>
      </LeftSide>
      <div
        style={{
          alignItems: "center",
          justifyContent: "center",
          display: "flex",
        }}
      >
        <iframe
          width="1280"
          height="720"
          src="https://www.youtube.com/embed/KJwYBJMSbPI"
          title="Demo video"
          frameborder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
          allowfullscreen
        ></iframe>
      </div>
    </>
  );
}


const LeftSide = styled.div`
  width: 50%;
  height: 100%;
  @media (max-width: 960px) {
    width: 100%;
    order: 2;
    margin: 50px 0;
    text-align: center;
  }
  @media (max-width: 560px) {
    margin: 80px 0 50px 0;
  }
`;

const HeaderP = styled.div`
  max-width: 470px;
  padding: 15px 0 50px 0;
  line-height: 1.5rem;
  @media (max-width: 960px) {
    padding: 15px 0 50px 0;
    text-align: center;
    max-width: 100%;
  }
`;

